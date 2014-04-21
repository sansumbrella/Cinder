
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

    if( aState & Button1Mask ) result |= MouseEvent::LEFT_DOWN;
    if( aState & Button2Mask ) result |= MouseEvent::MIDDLE_DOWN;
    if( aState & Button3Mask ) result |= MouseEvent::RIGHT_DOWN;

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
		
		handleXEvents();
	}

//	killWindow( mFullScreen );
	mApp->emitShutdown();
	delete mApp;
}

void AppImplLinuxBasic::handleXEvents()
{
	int _pendingEvents = XPending( cinder::Display::getMainDisplay()->getXDisplay() );
	while( XPending( cinder::Display::getMainDisplay()->getXDisplay() ) > 0 )
	{
		//std::cout << " Pending events " << _pendingEvents << std::endl;
		XEvent report;
		XNextEvent( cinder::Display::getMainDisplay()->getXDisplay(), &report );
        WindowImplLinuxBasic* impl;

        for( auto windowIt = mWindows.begin(); windowIt != mWindows.end(); ++windowIt )
        {
            // TODO: Check here if we actually found a valid window..
            if( report.xany.window == (*windowIt)->getXWindow() )
            {
                impl = (*windowIt);
                impl->mAppImpl->setWindow( impl->getWindow() );
                setWindow((*windowIt)->getWindow() );
            }
        }


		switch( report.type )
		{
            case MapNotify:

            break;
            
			case Expose:

			break;

            case ConfigureNotify:
            {
                        std::cout << " WIN " << report.xconfigure.window <<" RECEIVED CONFIGURE AKA RESIZE EVENT " << std::endl;
                        impl->mWindowWidth = report.xconfigure.width;
                        impl->mWindowHeight = report.xconfigure.height;
                        impl->mWindowOffset.y = report.xconfigure.x;
                        impl->mWindowOffset.y = report.xconfigure.y;
                        impl->getWindow()->emitResize();
            }
            break; 

			case ButtonPress:
            {
                    if( report.xbutton.button == Button1 )
                    {
                            MouseEvent event( impl->getWindow(), MouseEvent::LEFT_DOWN, report.xbutton.x, report.xbutton.y, prepModifiers(  report.xbutton.state ), 0.0f, report.xbutton.state );
                            impl->mIsDragging = true;
                            impl->getWindow()->emitMouseDown( &event );
                            std::cout << " Button 1 Press " << report.xbutton.window << " RECEIVED " << impl->getWindow() << std::endl;
                    }
                    else if( report.xbutton.button == Button2 )
                    {
                            MouseEvent event( impl->getWindow(), MouseEvent::MIDDLE_DOWN, report.xbutton.x, report.xbutton.y, prepModifiers(  report.xbutton.state ), 0.0f, report.xbutton.state );
                            impl->mIsDragging = true;
                            impl->getWindow()->emitMouseDown( &event );
                            std::cout << " Button 2 Press " << report.xbutton.window << " RECEIVED " <<  impl->getWindow() << std::endl;
                    }
                    else if( report.xbutton.button == Button3 )
                    {
                            MouseEvent event( impl->getWindow(), MouseEvent::RIGHT_DOWN, report.xbutton.x, report.xbutton.y, prepModifiers(  report.xbutton.state ), 0.0f, report.xbutton.state );
                            impl->mIsDragging = true;
                            impl->getWindow()->emitMouseDown( &event );
                            std::cout << " Button 3 Press " << report.xbutton.window << " RECEIVED " << impl->getWindow() << std::endl;
                    }
            }
			break;

            case ButtonRelease:
            {
                    impl->mIsDragging = false;
            }
            break;

            case MotionNotify:
            {
                    MouseEvent event( impl->getWindow(), 0, report.xmotion.x, report.xmotion.y, prepModifiers( report.xmotion.state ), 0.0f, report.xmotion.state );
                    if( impl->mIsDragging )
                    {
                      impl->getWindow()->emitMouseDrag( &event );
                      std::cout << " DRAGGING X "<< report.xmotion.x << " Y " << report.xmotion.y <<" WINDOW : " << impl->getWindow()<< std::endl;
                    } 
            }
            break;
			
			case KeyPress:
            {
                KeyEvent event( impl->getWindow(),0,0,0,0,KeyEvent::translateNativeKeyCode( report.xkey.keycode ) );
                impl->getWindow()->emitKeyDown( &event );
				//quit();
            }
			break;
			
			case ClientMessage:
            {
				if( report.xclient.data.l[0] == impl->mAtomDeleteWindow )
				{
                    XDestroyWindow( cinder::Display::getMainDisplay()->getXDisplay(), impl->getXWindow() );
                    closeWindow( impl );
				}
            }
			break;

			default:
			break;
		}
	} 	
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
