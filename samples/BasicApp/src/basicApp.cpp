#include "basicApp.h"
#include "cinder/Utilities.h"
#include <list>

// We'll create a new Cinder Application by deriving from the AppBasic class
void BasicApp::mouseUp( MouseEvent event )
{
}

void BasicApp::setup()
{
    DataSourceRef dataAssetRef = loadAsset("cinder.jpg");

    try{
        testImg = gl::Texture( loadImage( dataAssetRef ) );
    }
    catch(...){
        console() << "Unable to load the texture file ! " << std::endl;
    }

    std::string resourceContent = loadString( loadResource("hello.txt") );
    console() << "Resource txt content: " << resourceContent << std::endl;

    fs::path resourcePath = getResourcePath("hello.txt");
    console() << "Resource PATH " << resourcePath << std::endl;

    fs::path assetPath = getAssetPath("test.png");
    console() << "Asset PATH " << assetPath << std::endl;
}

void BasicApp::mouseDown( MouseEvent event )
{
    if( event.isShiftDown() )
    {
        console() << " Shift isPressed ! " << std::endl;
    }
    if( event.isControlDown() )
    {
        console() << " Control isPressed ! " <<std::endl;
    }
     if( event.isLeftDown() )
    {
        console() << " Mouse LEFT isPressed " << std::endl;
    }
    if( event.isMiddleDown() )
    {
        console() << " MOUSE MIDDLE isPressed " << std::endl;
    }
   if( event.isRightDown() )
    {
        console() << " MOUSE RIGHT isPressed" << std::endl;
    }
    if( event.isAltDown() )
    {
        console() << " ALT isPressed " << std::endl;
    }
}
void BasicApp::mouseDrag( MouseEvent event )
{
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
    //std::cout << " KEY DOWN " << std::endl;
}

void BasicApp::draw()
{
	gl::clear( Color( 0.0f, 0.0f, 0.0f ) );

    gl::color( 1.0f, 1.0f, 1.0f );
    if( testImg )
        gl::draw( testImg, getWindowBounds() );

	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}
	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( BasicApp, RendererGl )
