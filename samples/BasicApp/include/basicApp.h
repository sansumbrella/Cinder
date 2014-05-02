#pragma once 

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicApp : public AppBasic {
  public:
    void setup();
	void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event );
    void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
    gl::Texture     testImg ;
};

