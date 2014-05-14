/******************************************************************************
file: xwin_t.h
desc: 
 *****************************************************************************/
#pragma once

#include "rn_t.h"			//for renderer
#include <X11/X.h>			//x stuff
#include <X11/Xlib.h>		//	"
#include <GL/glew.h>		//extension enumerator (has to be here for link)
#include <GL/glx.h>			//for xvisualinfo
#include <GL/gl.h>			//	"

#define DEF_X_POS 256
#define DEF_Y_POS 256
#define DEF_WIDTH 640
#define DEF_HEIGHT 480
#define COLOR_DEPTH 24
#define WIN_NAME "xhghost"

class xwin_t
{
	public:
		/*functions*/
		xwin_t( void ) {};
		~xwin_t( void ) {};

		int start( void );
		int handle_events( rn_t* r );
		int shutdown( void );

	private:
		/*functions*/
		int make_window( int x, int y, unsigned w, unsigned h );
		int test_draw( rn_t* r );
		int destroy( void );

		/*attribs*/
		int x_x_pos;
		int x_y_pos;
		int x_width;
		int x_height;

		Display* x_disp;
		Window* x_root_win;
		Window* x_win;
		XVisualInfo* x_vis_info;
		XSetWindowAttributes* x_set_attribs;
		XWindowAttributes* x_attribs;
		XEvent*	x_event;
		Colormap* x_cmap;

		GLint x_gl_attribs[5];
		GLXContext* x_glcon;
};

