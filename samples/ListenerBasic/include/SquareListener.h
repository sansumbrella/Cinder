#pragma once

// Case sensitive file system
#ifdef __linux
    #include "cinder/app/App.h"
#else
    #include "cinder/App/App.h"
#endif

#include "cinder/Vector.h"

class SquareListener {
  public:
	SquareListener( ci::app::WindowRef window );
 
	void	mouseDown( ci::app::MouseEvent &event );
	void	mouseDrag( ci::app::MouseEvent &event );
	
	void	draw();
	
  private:
	ci::Rectf		mRect;
	bool			mSelected;
	
	ci::app::WindowRef				mWindow;
	ci::signals::scoped_connection	mCbMouseDown, mCbMouseDrag;
};
