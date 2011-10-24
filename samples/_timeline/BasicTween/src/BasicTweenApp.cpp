#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class BasicTweenApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void draw();
  
	Vec2f mBlackPos, mWhitePos;
};

void BasicTweenApp::setup()
{	
	mBlackPos = mWhitePos = getWindowCenter();
}

void BasicTweenApp::mouseDown( MouseEvent event )
{
	// the call to apply() replaces any existing tweens on mBlackPos with this new one
	getTimeline().apply( &mBlackPos, (Vec2f)event.getPos(), 2.0f, EaseInCubic() );
	// the call to appendTarget() ensures that any other tweens on mWhitePos will complete before this new one begins
	getTimeline().appendTarget( &mWhitePos, (Vec2f)event.getPos(), 3.0f, EaseOutQuint() );
}

void BasicTweenApp::draw()
{
	gl::clear( Color( 0.5f, 0.5f, 0.5f ) );
	
	gl::color( Color::black() );
	gl::drawSolidCircle( mBlackPos, 20.0f );
	
	gl::color( Color::white() );
	gl::drawSolidCircle( mWhitePos, 16.0f );
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( BasicTweenApp, RendererGl )
