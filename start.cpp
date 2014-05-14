/******************************************************************************
file: start.cpp
desc: 
 *****************************************************************************/
#include "common.h"
#include "xwin_t.h"
#include "rn_t.h"

/******************************************************************************
func: entry_t( void )
desc: 
 *****************************************************************************/
int main( int argc, char** argv )
{
	xwin_t* xw = new xwin_t;
	rn_t* r = new rn_t;

	assert( xw && r );

	xw->start();
	r->start();
	xw->handle_events( r );
	xw->shutdown();

	delete r;
	delete xw;

	return 0;
}

