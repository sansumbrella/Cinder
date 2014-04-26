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

#pragma once

#include "cinder/Stream.h"
#include "cinder/Vector.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/app/Renderer.h"
#include "cinder/Display.h"
#include "cinder/app/Window.h"
#include <string>
#include <vector>
#include <list>
#include <GLFW/glfw3.h>


namespace cinder { namespace app {


class AppImplLinux {
 public:

	AppImplLinux( class App *aApp );
	virtual ~AppImplLinux();
	
	class App*		    getApp() { return mApp; }

	float			    getFrameRate() const { return mFrameRate; }
	virtual float		setFrameRate( float aFrameRate ) { return -1.0f; }
	virtual void		quit() = 0;

	virtual WindowRef	getWindow() const { return mActiveWindow; }
	void			    setWindow( WindowRef window ) { mActiveWindow = window; }
	
	static void		    hideCursor();
	static void		    showCursor();
	
	static Buffer		loadResource( int id, const std::string &type );
	
	static fs::path		getAppPath();	
	static fs::path		getOpenFilePath( const fs::path &initialPath, std::vector<std::string> extensions );
	static fs::path		getSaveFilePath( const fs::path &initialPath, std::vector<std::string> extensions );
	static fs::path		getFolderPath( const fs::path &initialPath );
	
  protected:
	bool			    setupHasBeenCalled() const { return mSetupHasBeenCalled; }
	virtual void		closeWindow( class WindowImplLinux *windowImpl ) = 0;
	virtual void		setForegroundWindow( WindowRef window ) = 0;

	class App		    *mApp;
	float		    	mFrameRate;
	WindowRef		    mActiveWindow;
	bool			    mSetupHasBeenCalled;

	friend class        WindowImplLinux;
};

class WindowImplLinux {
  public:

	WindowImplLinux( const Window::Format &format, RendererRef sharedRenderer, AppImplLinux *appImpl );
	virtual ~WindowImplLinux() {}

	virtual bool		isFullScreen() { return mFullScreen; }
	virtual void		setFullScreen( bool fullScreen, const app::FullScreenOptions &options );
	virtual Vec2i		getSize() const { return Vec2i( mWindowWidth, mWindowHeight ); }
	virtual void		setSize( const Vec2i &size );
	virtual Vec2i		getPos() const { return mWindowOffset; }
	virtual void		setPos( const Vec2i &pos );
	virtual void		close();
	virtual std::string	getTitle() const;
	virtual void		setTitle( const std::string &title );
	virtual void		hide();
	virtual void		show();
	virtual bool		isHidden() const;
	virtual DisplayRef	getDisplay() const { return mDisplay; }
	virtual RendererRef	getRenderer() const { return mRenderer; }
    // NULL for now until I figure out how to grab the X window from GLFW.
	virtual void*		getNative() { return NULL; }

	bool			    isBorderless() const { return mBorderless; }
	void			    setBorderless( bool borderless );
	bool			    isAlwaysOnTop() const { return mAlwaysOnTop; }
	void			    setAlwaysOnTop( bool alwaysOnTop );

	AppImplLinux*		getAppImpl() { return mAppImpl; }
	WindowRef		    getWindow() { return mWindowRef; }
	virtual void		keyDown( const KeyEvent &event );
	virtual void		draw();
	virtual void		redraw();
	virtual void		resize();
    virtual void        mouseMove( int x, int y );
    virtual void        mousePressed( int button, int action, int mods );
    virtual void        keyPressed( int key, int scancode, int action, int mod );
    virtual void        keyCharPressed( unsigned int aChar );
	virtual void		privateClose();
  protected:
	void			    createWindow( const Vec2i &windowSize, const std::string &title, DisplayRef display, RendererRef sharedRenderer );
	void			    getScreenSize( int clientWidth, int clientHeight, int *resultWidth, int *resultHeight );
	virtual void		toggleFullScreen( const app::FullScreenOptions &options );

	AppImplLinux		*mAppImpl;
	WindowRef		    mWindowRef;
	Vec2i			    mWindowOffset;
	bool			    mHidden;
    bool                mIsDragging;
    int				    mWindowWidth, mWindowHeight;
	bool			    mFullScreen, mBorderless, mAlwaysOnTop, mResizable;
	Vec2i			    mWindowedPos, mWindowedSize;
	DisplayRef		    mDisplay;
	RendererRef		    mRenderer;

    inline static void window_close_callback( GLFWwindow* aGLFWwindow )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) ); 
        _currentWindow->close();
    }

    inline static void window_pos_callback( GLFWwindow* aGLFWwindow, int xpos, int ypos )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) );
        _currentWindow->mWindowOffset.x = xpos;
        _currentWindow->mWindowOffset.y = ypos;
    }

    inline static void window_resize_callback( GLFWwindow* aGLFWwindow, int width, int height )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) );
        _currentWindow->mWindowWidth = width;
        _currentWindow->mWindowHeight = height;
        _currentWindow->resize();
    }

    inline static void mouse_move_callback( GLFWwindow* aGLFWwindow, double xpos, double ypos )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) );
        _currentWindow->mouseMove( xpos, ypos );
    }

    inline static void mouse_pressed_callback( GLFWwindow* aGLFWwindow, int button, int action, int mods )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>( glfwGetWindowUserPointer( aGLFWwindow ) );   
        _currentWindow->mousePressed( button, action, mods );
    }
    
    inline static void key_action_callback( GLFWwindow* aGLFWwindow, int key, int scancode, int action, int mods )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) );
        if( action == GLFW_PRESS && glfwGetWindowAttrib( aGLFWwindow, GLFW_FOCUSED) )
        {
            _currentWindow->keyPressed( key, scancode, action, mods );
        }
    }

    inline static void key_character_callback( GLFWwindow* aGLFWwindow, unsigned int aChar )
    {
        WindowImplLinux* _currentWindow = static_cast<WindowImplLinux*>(glfwGetWindowUserPointer( aGLFWwindow ) );
        if( glfwGetWindowAttrib( aGLFWwindow, GLFW_FOCUSED) )
        {
            _currentWindow->keyCharPressed( aChar );
        }

    }

    virtual unsigned int prepMouseEventModifiers( int aButton, int aMod );

	friend AppImplLinux;
};


} } // namespace cinder::app
