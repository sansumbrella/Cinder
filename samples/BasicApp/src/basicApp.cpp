#include "cinder/app/AppBasic.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public AppBasic {
  public:
	void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event );
    void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
};

void BasicApp::mouseUp( MouseEvent event )
{
}
void BasicApp::mouseDown( MouseEvent event )
{
    if( event.isShiftDown() )
    {
        std::cout << " Shift isPressed ! " << std::endl;
    }
    if( event.isControlDown() )
    {
        std::cout << " Control isPressed ! " <<std::endl;
    }
     if( event.isLeftDown() )
    {
        std::cout << " Mouse LEFT isPressed " << std::endl;
    }
    if( event.isMiddleDown() )
    {
        std::cout << " MOUSE MIDDLE isPressed " << std::endl;
    }
   if( event.isRightDown() )
    {
        std::cout << " MOUSE RIGHT isPressed" << std::endl;
    }
    if( event.isAltDown() )
    {
        std::cout << " ALT isPressed " << std::endl;
    }
}
void BasicApp::mouseDrag( MouseEvent event )
{
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
    std::cout << " KEY DOWN " << std::endl;
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void BasicApp::draw()
{
	gl::clear( Color( 0.1f, 0.1f, 0.15f ) );

	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}
	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( BasicApp, RendererGl )
