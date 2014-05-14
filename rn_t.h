/******************************************************************************
file: rn_t.h
desc: 
 *****************************************************************************/
#pragma once

#include <GL/glew.h>		//extension enumerator (has to be here for link)
#include <GL/gl.h>			//gl types
#include <vector>			//stl vector

#define BG_COLOR 0.0f, 0.5f, 1.0f, 0.0f

using std::vector;

class rn_t
{
	public:
		/*functions*/
		rn_t( void ) {};
		~rn_t( void ) {};

		int start( void );
		int run( void );
		int shutdown( void ) { return 0; };

	private:
		/*functions*/
		int load_all_shaders( void );
		int compile_shader( GLuint *sh_num, GLenum sh_type, char* sh_file );
		int create_sh_program( void );
		int delete_all_shaders( void );
		int init_vtx_b( void );

		/*attribs*/
		vector<GLuint> r_shader_list;
		GLuint r_sh_program;

		GLuint r_pos_bobj;
		GLuint r_vtx_array_obj;
};


