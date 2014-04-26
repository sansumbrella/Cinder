
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

#include "cinder/app/AppImplLinuxBasic.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppImplLinuxRenderer.h"
#include "cinder/app/Renderer.h"
#include "cinder/Utilities.h"

#include <GLFW/glfw3.h>

using std::vector;
using std::string;

namespace cinder { namespace app {

AppImplLinuxBasic::AppImplLinuxBasic( AppBasic *aApp )
	: AppImplLinux( aApp ), mApp( aApp )
{
	mShouldQuit = false;
}

unsigned int AppImplLinuxBasic::prepModifiers( unsigned int aState )
{
    unsigned int result = 0;

    return result;
}

void AppImplLinuxBasic::run()
{
	mFrameRate = mApp->getSettings().getFrameRate();
	mFrameRateEnabled = mApp->getSettings().isFrameRateEnabled();

	auto formats = mApp->getSettings().getWindowFormats();
	if( formats.empty() )
		formats.push_back( mApp->getSettings().getDefaultWindowFormat() );
	for( auto format = formats.begin(); format != formats.end(); ++format ) {
		if( ! format->isTitleSpecified() )
			format->setTitle( mApp->getSettings().getTitle() );
		createWindow( *format );
	}

	mApp->privateSetup__();
	mSetupHasBeenCalled = true;

	for( auto windowIt = mWindows.begin(); windowIt != mWindows.end(); ++windowIt )
		(*windowIt)->resize();

	// initialize our next frame time
	mNextFrameTime = getElapsedSeconds();

	// inner loop
	while( ! mShouldQuit ) {
		// update and draw
		mApp->privateUpdate__();
		for( auto windowIt = mWindows.begin(); windowIt != mWindows.end(); ++windowIt )
		{
			(*windowIt)->draw();
		}
		// get current time in seconds
		double currentSeconds = mApp->getElapsedSeconds();

		if( ( currentSeconds >= mNextFrameTime ) )
		{
			mNextFrameTime = currentSeconds + 1.0f / mFrameRate;
		}
		else 
		{
			usleep(1);
		}
		
        glfwPollEvents();
	}

//	killWindow( mFullScreen );
	mApp->emitShutdown();
	delete mApp;

    glfwTerminate();
}


void AppImplLinuxBasic::sleep( double seconds )
{
}


WindowRef AppImplLinuxBasic::createWindow( Window::Format format )
{
	if( ! format.getRenderer() )
		format.setRenderer( mApp->getDefaultRenderer()->clone() );

	mWindows.push_back( new WindowImplLinuxBasic( format, mApp->findSharedRenderer( format.getRenderer() ), this ) );

	// emit initial resize if we have fired setup
	if( mSetupHasBeenCalled )
		mWindows.back()->getWindow()->emitResize();

	return mWindows.back()->getWindow();
}

void AppImplLinuxBasic::closeWindow( WindowImplLinux *windowImpl )
{
	auto winIt = find( mWindows.begin(), mWindows.end(), windowImpl );
	if( winIt != mWindows.end() ) {
		windowImpl->getWindow()->emitClose();
		windowImpl->privateClose();
		delete windowImpl; // this corresponds to winIt
		mWindows.erase( winIt );
	}

	if( mWindows.empty() && mApp->getSettings().isQuitOnLastWindowCloseEnabled() )
		mShouldQuit = true;
}

size_t AppImplLinuxBasic::getNumWindows() const
{
	return mWindows.size();
}

WindowRef AppImplLinuxBasic::getWindowIndex( size_t index )
{
	if( index >= mWindows.size() )
		return cinder::app::WindowRef();
	
	std::list<WindowImplLinuxBasic*>::iterator iter = mWindows.begin();
	std::advance( iter, index );
	return (*iter)->mWindowRef;
}

WindowRef AppImplLinuxBasic::getForegroundWindow() const
{
	return mForegroundWindow;
}

void AppImplLinuxBasic::setForegroundWindow( WindowRef window )
{
	mForegroundWindow = window;
}

float AppImplLinuxBasic::setFrameRate( float aFrameRate )
{
	mFrameRate = aFrameRate;
	mFrameRateEnabled = true;
	mNextFrameTime = mApp->getElapsedSeconds();
	return aFrameRate;
}

void AppImplLinuxBasic::disableFrameRate()
{
	mFrameRateEnabled = false;
}

bool AppImplLinuxBasic::isFrameRateEnabled() const
{
	return mFrameRateEnabled;
}

///////////////////////////////////////////////////////////////////////////////
// WindowImplLinuxBasic
void WindowImplLinuxBasic::toggleFullScreen( const app::FullScreenOptions &options )
{
	WindowImplLinux::toggleFullScreen( options );
}

} } // namespace cinder::app
