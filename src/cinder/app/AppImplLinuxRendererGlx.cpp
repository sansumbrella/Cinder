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

#include "cinder/app/AppImplLinuxRendererGlx.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/Camera.h"

namespace cinder { namespace app {

bool sMultisampleSupported = false;
int sArbMultisampleFormat;

AppImplLinuxRendererGlx::AppImplLinuxRendererGlx( App *aApp, RendererGl *aRenderer )
	: AppImplLinuxRenderer( aApp ), mRenderer( aRenderer )
{
	std::cout << " Creating LINUX Renderer" << this << " GL CONTEXT " << &mGlc <<std::endl;
}

void AppImplLinuxRendererGlx::defaultResize() const
{
    std::cout << "DEFAULT RESIZE WITH W : " << mApp->getWindow()->getWidth() << " H : " << mApp->getWindow()->getHeight() << " ON WINDOW " << mApp->getWindow() <<std::endl;
    glViewport( 0, 0, mApp->getWindow()->getWidth(), mApp->getWindow()->getHeight() );
    cinder::CameraPersp cam( mApp->getWindow()->getWidth(), mApp->getWindow()->getHeight(), 60.0f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( cam.getProjectionMatrix().m );

    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixf( cam.getModelViewMatrix().m );

    glScalef( 1.0f, -1.0f, 1.0f );
    glTranslatef( 0.0f, (float)-mApp->getWindow()->getHeight(), 0.0f );
}

void AppImplLinuxRendererGlx::swapBuffers() const
{
	glXSwapBuffers( mDpy, *mWnd );
}

void AppImplLinuxRendererGlx::makeCurrentContext()
{
	glXMakeCurrent( mDpy, *mWnd, mGlc );
}

bool AppImplLinuxRendererGlx::initialize( xwindow::_XWindow & wnd, _XDisplay *dpy, XVisualInfo *aVisInfo, RendererRef sharedRenderer )
{
	std::cout << "INIT GLX " << std::endl;
	mWnd = &wnd;
	mDpy = dpy;
	mVisInfo = aVisInfo;
	return initializeInternal( mWnd, mDpy, mVisInfo );
}

bool AppImplLinuxRendererGlx::initializeInternal( xwindow::_XWindow *wnd, _XDisplay *dpy, XVisualInfo *aVisInfo )
{
	if( ! mVisInfo )
	{
        std::cout << " The XVisualInfo is empty ! Window will not be initialized... " << std::endl;
		return false;
	}

	if( ! ( mGlc = glXCreateContext( mDpy, mVisInfo, 0, GL_TRUE ) ) )
	{
        std::cout << " Failed to create glXContext ! Window will not be initialized... " << std::endl;
		return false;
	}
    else
    {
        std::cout << " Created context --> " << &mGlc << std::endl;
    }

	std::cout << "INITIALIZED SUCCESSFULLY GLX" << std::endl;
	
    return true;
}

int AppImplLinuxRendererGlx::initMultisample( )
{
}

void AppImplLinuxRendererGlx::kill()
{
	std::cout << " Destroying context ! "<< &mGlc << std::endl;
	glXMakeCurrent( mDpy, 0, 0 );
	glXDestroyContext( mDpy, mGlc );
}

} }
