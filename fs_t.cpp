/******************************************************************************
file: fs_t.cpp
desc: 
 *****************************************************************************/

#include "fs_t.h"		//
#include "common.h"		//

#include <vector>		//stl vector
#include <string>		//stl string
#include <fstream>		//all file io using fstreams
#include <sstream>		//for coverting streams
#include <iostream>		//remove

#include <dirent.h>		//for reading directories 
#include <sys/stat.h>	//S_ISDIR() - S_ISREG()
#include <string.h>		//memset
#include <unistd.h>		//getcwd()

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::fstream;
using std::stringstream;
using std::ostringstream;

/******************************************************************************
func:
desc: 
 *****************************************************************************/
int fs_t::get_all_files_in_dir( string path, vector<string>* dst )
{
	int retv = ERR_OK;
	char b[MAX_PATH] = { '\0' };
	DIR* d = NULL;
	struct dirent* dir = NULL;
	struct stat dstat = { 0 };
	assert( path.size() && dst );

	/*open the directory*/
	if( !( d = opendir( path.c_str() ) ) )
	{
		retv = ERR_OPENING_DIR;
		goto out;
	}

	/*get the files*/
	while( ( dir = readdir( d ) ) != NULL )
	{
		/*build a path*/
		memset( b, 0, MAX_PATH );
		getcwd( b, MAX_PATH );
		strcat( b, "/" );
		strcat( b, path.c_str() );
		strcat( b, dir->d_name );
		strcat( b, "\0" );

		/*check it its a file or directory*/
		if( ( stat( b, &dstat ) ) == - 1 )
		{
			retv = ERR_STAT;
			goto out;
		}

		/*skips . and ..*/
	    //if( !strcmp( dir->d_name, ".") || !strcmp( dir->d_name, ".." ) )
		//	continue;

		if( S_ISREG( dstat.st_mode ) )
		{
			string s( dir->d_name );
			dst->push_back( s );
		}
	}
	closedir( d );
	
out:
	return retv;
}

/******************************************************************************
func:
desc: 
 *****************************************************************************/
int fs_t::read_file_to_string( string filename, string* target )
{
	int retv = ERR_OK;
	fstream new_file;
	ostringstream convert;
	assert( filename.size() && target );

	/*open the file*/
	new_file.open( filename.c_str(), fstream::in );

	/*convert it*/
	convert << new_file.rdbuf();

	/*append to the current string*/
	*target = convert.str();

	/*close it*/
	new_file.close();

	return retv;
}

