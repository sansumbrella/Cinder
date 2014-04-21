/*
 Copyright (c) 2012, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/app/AppImplLinux.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/Display.h"
#include <GL/glx.h>

using std::string;
using std::wstring;
using std::vector;
using std::pair;

namespace cinder { namespace app {


AppImplLinux::AppImplLinux( App *aApp )
	: mApp( aApp ), mSetupHasBeenCalled( false )
{
}

AppImplLinux::~AppImplLinux()
{
}

void AppImplLinux::hideCursor()
{
}

void AppImplLinux::showCursor()
{
}

Buffer AppImplLinux::loadResource( int id, const std::string &type )
{
}

fs::path AppImplLinux::getAppPath()
{

	return fs::path( std::string( "test/App/path" ) );
}

fs::path AppImplLinux::getOpenFilePath( const fs::path &initialPath, vector<string> extensions )
{
		return string();
}


fs::path AppImplLinux::getFolderPath( const fs::path &initialPath )
{
	string result;


	return result;
}

fs::path AppImplLinux::getSaveFilePath( const fs::path &initialPath, vector<string> extensions )
{
	string result;

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// WindowImplLinux
WindowImplLinux::WindowImplLinux( const Window::Format &format, RendererRef sharedRenderer, AppImplLinux *appImpl )
	: mWindowOffset( 0, 0 ), mAppImpl( appImpl ), mHidden( false ), mIsDragging(false)
{	
	mFullScreen = format.isFullScreen();
	mDisplay = format.getDisplay();
	mRenderer = format.getRenderer();
	std::cout << " RENDERER " << mRenderer << std::endl;
	mResizable = format.isResizable();
	mAlwaysOnTop = format.isAlwaysOnTop();
	mBorderless = format.isBorderless();

	mWindowedSize = format.getSize();
	mWindowWidth = mWindowedSize.x;
	mWindowHeight = mWindowedSize.y;
	if( format.isPosSpecified() )
		mWindowOffset = mWindowedPos = format.getPos();
	else {
		Vec2i displaySize = mDisplay->getSize();
		mWindowOffset = mWindowedPos = ( displaySize - mWindowedSize ) / 2;
	}

	createWindow( Vec2i( mWindowWidth, mWindowHeight ), format.getTitle(), mDisplay, sharedRenderer );
	// set WindowRef and its impl pointer to this
	mWindowRef = Window::privateCreate__( this, mAppImpl->getApp() );
	mAppImpl->setWindow( mWindowRef );
	
}


void WindowImplLinux::createWindow( const Vec2i &windowSize, const std::string &title, const DisplayRef display, RendererRef sharedRenderer )
{
	int screen_num;
	unsigned long background, border;
	
	XEvent ev;
	_XDisplay *dpy = display->getXDisplay();

    // Most parts from here: www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)#The_Code
	int visual_attribs[] = 
	{
        GLX_X_RENDERABLE    ,   True,
        GLX_DRAWABLE_TYPE   ,   GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     ,   GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE	, 	GLX_TRUE_COLOR,
		GLX_RED_SIZE		,	8,
		GLX_GREEN_SIZE		,	8,
		GLX_BLUE_SIZE		,	8,
        GLX_ALPHA_SIZE      ,   8,
        GLX_STENCIL_SIZE    ,   8,
		GLX_DEPTH_SIZE		,	24,
		GLX_DOUBLEBUFFER	,	True,
		None
	
	};
	int glx_major, glx_minor;
    int fbcount;

    if( !glXQueryVersion( dpy, &glx_major, &glx_minor ) ||  ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
    {
        std::cout << "Invalid GLX version " << std::endl;
    }

	GLXFBConfig* fbc = glXChooseFBConfig( dpy, DefaultScreen(dpy), visual_attribs, &fbcount );
	
	if( !fbc )
	{
		std::cout << "Failed to retrieve a framebuffer config ! " << std::endl;
	}
	
	std::cout << "Found " << fbcount << " matching FB configs .. " << std::endl;
	
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
	
	for( int i = 0; i < fbcount; ++i )
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig( dpy, fbc[i] );	
		if( vi )
		{
			int samp_buf = 0, samples = 0;
			glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLES, &samples );	
			std::cout << " Matching fbconfig " << i << " with visual id " << vi->visualid << " SAMPLE_BUFFERS "<< samp_buf << " SAMPLES "<< samples << std::endl;
			if( best_fbc < 0 || samp_buf && samples > best_num_samp )
			{
				best_fbc = i, best_num_samp = samples;
			}
			
			if( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
			{
				worst_fbc = i, worst_num_samp = samples;
			}
		}
		XFree(vi);
	}
	
	GLXFBConfig bestFbc = fbc[ best_fbc ];
	
	XFree( fbc );

	XVisualInfo *visinfo = glXGetVisualFromFBConfig( dpy, bestFbc );

	std::cout << "Chosen visual "<<visinfo->visualid << std::endl;

	if( ! visinfo )
	{
		return ;
	}

	if( !dpy )
	{
		std::cout << "Unable to connect to display ! " << std::endl;
        return ;
	}
	
	screen_num = DefaultScreen(dpy);
	
	XSetWindowAttributes winattr;
	winattr.background_pixel = 0;
	winattr.border_pixel = 0;
	winattr.colormap = XCreateColormap( dpy, RootWindow (dpy, screen_num ), visinfo->visual, AllocNone ); 
	winattr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask  | KeyReleaseMask | LeaveWindowMask | FocusChangeMask | VisibilityChangeMask | EnterWindowMask | LeaveWindowMask | PropertyChangeMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask ;
	unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	mWnd = XCreateWindow( dpy, RootWindow (dpy, screen_num ), 0, 0, windowSize.x, windowSize.y, 0, visinfo->depth, InputOutput, visinfo->visual, mask, &winattr );
	
    if( !mWnd ) std::cout << "Failed to create XWindow ! " << std::endl;
    else std::cout << "Created window " << " X " << windowSize.x << " Y " << windowSize.y << " ADDRESS " << &mWnd <<  std::endl;
	
	char windowTitle[1024];
	std::strncpy( windowTitle, title.c_str(), sizeof(windowTitle) );
	windowTitle[sizeof(windowTitle) - 1] = 0;
	XStoreName( dpy, mWnd, windowTitle );

	XMapRaised( dpy, mWnd );
	XMapWindow( dpy, mWnd );
	
	mAtomDeleteWindow = XInternAtom( dpy, "WM_DELETE_WINDOW", False );
	XSetWMProtocols( dpy, mWnd, &mAtomDeleteWindow, 1 );	
	
	mRenderer->setup( mAppImpl->getApp(), mWnd, dpy, visinfo, sharedRenderer );
}


void WindowImplLinux::setFullScreen( bool fullScreen, const app::FullScreenOptions &options )
{
	if( mFullScreen != fullScreen )
		toggleFullScreen( options );
}

void WindowImplLinux::toggleFullScreen( const app::FullScreenOptions &options )
{
}

void WindowImplLinux::getScreenSize( int clientWidth, int clientHeight, int *resultWidth, int *resultHeight )
{
}

void WindowImplLinux::setPos( const Vec2i &windowPos )
{
}

void WindowImplLinux::hide()
{
}

void WindowImplLinux::show()
{
}

bool WindowImplLinux::isHidden() const
{
	return mHidden;
}

std::string	WindowImplLinux::getTitle() const
{
	return "";
}

void WindowImplLinux::setTitle( const std::string &title )
{
	std::wstring titleWide = toUtf16( title );
}

void WindowImplLinux::setSize( const Vec2i &windowSize )
{
	int screenWidth, screenHeight;
	getScreenSize( windowSize.x, windowSize.y, &screenWidth, &screenHeight );
}

void WindowImplLinux::close()
{
	getAppImpl()->closeWindow( this );
	// at this point 'this' is invalid so do nothing after this line
}

void WindowImplLinux::setBorderless( bool borderless )
{
}
 
void WindowImplLinux::setAlwaysOnTop( bool alwaysOnTop )
{
}

void WindowImplLinux::draw()
{
	mAppImpl->setWindow( mWindowRef );
	mRenderer->startDraw();
	mWindowRef->emitDraw();
	mRenderer->finishDraw();
}

void WindowImplLinux::resize()
{
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitResize();
}

void WindowImplLinux::redraw()
{
}

void WindowImplLinux::privateClose()
{
	mRenderer->kill();
}

void WindowImplLinux::keyDown( const KeyEvent &event )
{
	KeyEvent localEvent( event );
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitKeyDown( &localEvent );
}

} } // namespace cinder::app
