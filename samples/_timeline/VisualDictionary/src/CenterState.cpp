//
//  CenterState.cpp
//  VisualDictionary
//
//  Created by Robert Hodgin on 11/2/11.
//  Copyright (c) 2011. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "CenterState.h"
#include "cinder/gl/gl.h"

#include <list>
#include <algorithm>

using namespace std;
using namespace ci;

gl::TextureFontRef CenterState::sFont;

CenterState::CenterState( float radius )
	: mRadius( radius )
{
}


void CenterState::addCircle(  const string &word, const Color &color )
{
	mWord				= word;
	mWordPixelLength	= sFont->measureString( mWord ).x;
	mCounter			= 0;
	
	// draw the bg circles and animate their color and radius
	for( list<Circle>::reverse_iterator circleIt = mCircles.rbegin(); circleIt != mCircles.rend(); ++circleIt ){
		circleIt->mRadiusDest += 10.0f;
		app::timeline().apply( &circleIt->mRadius, circleIt->mRadiusDest + 25.0f, 0.2f, EaseInOutQuad() ).timelineEnd( - 0.375f );
		app::timeline().appendTo( &circleIt->mRadius, circleIt->mRadiusDest, 0.2f, EaseInOutQuad() );
	}
	
	mCircles.push_back( Circle( 140.0f, color ) );
	app::timeline().apply( &mCircles.back().mRadius, mCircles.back().mRadiusDest, 0.2f, EaseInQuad() );
}

void CenterState::update( const WordNode &currentNode )
{
	int numCircles = mCircles.size();
	
	int index = 1;
	for( list<Circle>::reverse_iterator circleIt = mCircles.rbegin(); circleIt != mCircles.rend(); ++circleIt ){
		if( mCounter%( numCircles * 6 ) == index * 4 ){
			app::timeline().apply( &circleIt->mRadius, circleIt->mRadiusDest + 30.0f, 0.25f, EaseInOutQuad() );
			app::timeline().appendTo( &circleIt->mRadius, circleIt->mRadiusDest, 0.25f, EaseInOutQuad() );
		}
		index ++;
	}

	
	mCounter ++;
}

void CenterState::draw( const Vec2f &center )
{
	for( list<Circle>::iterator circleIt = mCircles.begin(); circleIt != mCircles.end(); ++circleIt ){
		circleIt->draw( center );
	}
	
	float radius = 140.0f;
	
	// draw string
	// biggest square that can fit in the circle is radius * sqrt(2) per side  x^2 = (r^2)/2
	const float squareSide = sqrtf( ( radius * radius ) / 2.0f );
	
	float pixelScale = std::min( squareSide / mWordPixelLength, squareSide / 150 ) * 2.0f;
	gl::TextureFont::DrawOptions options = gl::TextureFont::DrawOptions().scale( pixelScale ).pixelSnap( false );
		
	const Vec2f offset = center + Vec2f( -radius + ( radius * 2 - mWordPixelLength * pixelScale ) / 2, radius - (radius * 2.0f - 60 * pixelScale ) / 2 );
		
	gl::color( ColorA( Color::black(), 0.5f ) );
	sFont->drawString( mWord, offset + Vec2f( pixelScale, pixelScale ) * 1.5f, options );
		
	gl::color( ColorA( Color::white(), 1.0f ) );
	sFont->drawString( mWord, offset, options );
}

void CenterState::setFont( gl::TextureFontRef font )
{
	sFont = font;
}