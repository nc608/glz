/******************************************************************************
file: fs_t.h
desc: 
 *****************************************************************************/
#pragma once

#include <vector>		//stl vector
#include <string>		//stl string
#include <fstream>		//all file io using fstreams

using std::vector;
using std::string;
using std::fstream;

class fs_t
{
	public:
		fs_t( void ) {};
		~fs_t( void ) {};

		/*functions*/
		int start( void ) { return 0; };
		int shutdown( void ) { return 0; };
		int get_all_files_in_dir( string path, vector<string>* dst );
		int read_file_to_string( string filename, string* target );
		int read_file_to_buffer( string filename, char* b, unsigned* bsz ) {  return 0; };
		
		int close_all_open_buffers( void ) { return 0; };


	private:	
		vector<char*> f_created_buffers;
		vector<fstream> f_open_fstreams;
};



