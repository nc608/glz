/******************************************************************************
file: rn_t.cpp
desc: 
 *****************************************************************************/
#include "rn_t.h"			//
#include "common.h"			//
#include "fs_t.h"			//filesystem
#include <GL/glew.h>		//extension enumerator
#include <GL/gl.h>			//gl types
#include <iostream>			//cout
#include <vector>			//stl vector
#include <algorithm>		//stl for each
#include <string>			//stl string

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::for_each;

/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::start( void )
{
	int retv = ERR_OK;
	GLenum gl_err = glewInit();

	fs_t f;
	string blank_string = "";
	vector<string> files;
	vector<string>::iterator i = files.begin();

	/*start glew*/
	if( gl_err != GLEW_OK )
	{
		cout << "error starting glew (" << glewGetErrorString( gl_err );
		cout << ")" <<endl;
		retv = ERR_GLEW;
		goto out;
	}

	cout << "glew version " << glewGetString( GLEW_VERSION );
	cout << " started successfully" << endl;

	if( ( retv = load_all_shaders() ) != ERR_OK )
		goto out;

	if( ( retv = create_sh_program() ) != ERR_OK )
		goto out;

	delete_all_shaders();
	init_vtx_b();

	cout << "testing filesystem" << endl << endl;
	f.read_file_to_string( "shader/file_vtx_shader.c", &blank_string );
	f.get_all_files_in_dir( "shader/", &files );

	for( i = files.begin(); i != files.end(); i ++ )
		cout << *i << ", ";

	cout << endl;


out:
	return retv;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::run( void )
{
	int retv = ERR_OK;

	glClearColor( BG_COLOR );
	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram( r_sh_program );
	glBindBuffer( GL_ARRAY_BUFFER, r_pos_bobj );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer(
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			0 );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glDisableVertexAttribArray( 0 );
	glUseProgram( 0 );
	return retv;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::shutdown( void )
{
	/*shut down*/
	glDeleteProgram( r_sh_program );
	return ERR_OK;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::load_all_shaders( void )
{
	int retv = ERR_OK;
	GLuint sh_num = 0;

	string str_vtx_shader(
			"#version 120\n"
			"vec4 position;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = ftransform();\n"
			"}\n" );

	string str_frag_shader(
			"#version 120\n"
			"vec4 outputColor;\n"
			"void main()\n"
			"{\n"
			"   gl_FragColor = vec4( 0.0f, 0.05f, 0.1f, 1.0f );\n"
			"}\n" );

	compile_shader(
			&sh_num,
			GL_VERTEX_SHADER,
			(char*)str_vtx_shader.c_str() );
	r_shader_list.push_back( sh_num );

	compile_shader(
			&sh_num,
			GL_FRAGMENT_SHADER,
			(char*)str_frag_shader.c_str() );
	r_shader_list.push_back( sh_num );

	return retv;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::compile_shader( GLuint* sh_num, GLenum sh_type, char* sh_file )
{
	int retv = ERR_OK;
	GLuint shader = glCreateShader( sh_type );
	GLint status = 0;
	GLint log_length = 0;
	GLchar* shader_err_log = NULL;
	assert( sh_num && sh_file && shader );

	/*compile the shader*/
	glShaderSource( shader, 1, (const char**)&sh_file, NULL );
	glCompileShader( shader );

	/*check for errors*/
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if( status == GL_FALSE )
	{
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_length );
		shader_err_log = new GLchar[log_length + 1];
		glGetShaderInfoLog( shader, log_length, NULL, shader_err_log );

		cout << "error compiling shader (";

		switch( sh_type )
		{
			case GL_VERTEX_SHADER: cout << "vertex"; break;
			case GL_GEOMETRY_SHADER: cout << "geometry"; break;
			case GL_FRAGMENT_SHADER: cout << "fragment"; break;
			default: cout << "unknown"; break;
		}

		cout << ")" << endl << shader_err_log << endl;
		delete[] shader_err_log;
		shader_err_log = NULL;
		retv = ERR_SHADER_COMPILE;
		goto out;
	}

out:
	*sh_num = shader;
	return retv;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::create_sh_program( void )
{
	int retv = ERR_OK;
	GLint glerr = 0;
	GLint glerr_len = 0;
	GLchar* glerr_log = NULL;
	vector<GLuint>::iterator i = r_shader_list.begin();

	r_sh_program = glCreateProgram();

	/*attach all the shaders to the program*/
	while( i != r_shader_list.end() )
	{
		glAttachShader( r_sh_program, *i );
		i++;
	}

	/*needed for 1.20*/
	glBindAttribLocation( r_sh_program, 0, "position" );

	/*link it*/
	glLinkProgram( r_sh_program );

	/*check for errors*/
	glGetProgramiv( r_sh_program, GL_LINK_STATUS, &glerr );
	if( glerr == GL_FALSE )
	{
		glGetProgramiv( r_sh_program, GL_INFO_LOG_LENGTH, &glerr_len );
		glerr_log = new GLchar[glerr_len + 1];
		glGetProgramInfoLog( r_sh_program, glerr_len, NULL, glerr_log );
		cout << "shader linker failure: " << glerr_log << endl;
		delete[] glerr_log;
		glerr_log = NULL;
		retv = ERR_SHADER_LINK;
	}

	/*detach the shaders*/
	for( i = r_shader_list.begin(); i != r_shader_list.end(); i++ )
		glDetachShader( r_sh_program, *i );

	return retv;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::delete_all_shaders( void )
{
	for_each( r_shader_list.begin(), r_shader_list.end(), glDeleteShader );
	return ERR_OK;
}


/******************************************************************************
func:
desc: 
 *****************************************************************************/
int rn_t::init_vtx_b( void )
{
	/*tri data*/
	float vtx_pos[] =
	{
		0.75f, 0.75f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f,
		-0.75f, -0.75f, 0.0f, 1.0f,
	};

	/*bind the buffers to the gfx card*/
	glGenBuffers( 1, &r_pos_bobj );
	glBindBuffer( GL_ARRAY_BUFFER, r_pos_bobj );
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(vtx_pos),
			vtx_pos,
			GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	/*???*/
	glGenVertexArrays( 1, &r_vtx_array_obj );
	glBindVertexArray( r_vtx_array_obj );

	return ERR_OK;
}

