//
//  CenterState.h
//  VisualDictionary
//
//  Created by Robert Hodgin on 11/2/11.
//  Copyright (c) 2011. All rights reserved.
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/TextureFont.h"

#include <string>

#include "WordNode.h"

struct Circle {
public:
	Circle( float radius, ci::Color color )
	: mRadius( 0.0f ), mRadiusDest( radius ), mColor( color )
	{}
	void draw( const ci::Vec2f &pos ) {
		ci::gl::color( ci::ColorA( 0, 0, 0, 0.5f ) );
		
		ci::Vec2f p = pos;
		p += ci::Vec2f( 1.0f, 1.0f );
		ci::gl::drawSolidRect( ci::Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );
		p += ci::Vec2f( 1.0f, 1.0f );
		ci::gl::drawSolidRect( ci::Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );
		
		// color foreground
		ci::gl::color( mColor );
		p	= pos;
		ci::gl::drawSolidRect( ci::Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );
	}
	
	ci::Anim<float>		mRadius;
	ci::Anim<ci::Color>	mColor;
	float				mRadiusDest;
};


class CenterState {
public:
	CenterState(){}
	CenterState( float radius );
	
	const std::string&	getWord() const { return mWord; }

	void	addCircle(  const std::string &word, const ci::Color &color );
	void	update( const WordNode &currentNode );
	void	draw( const ci::Vec2f &center );
	
	std::list<Circle>	mCircles;
	
	static	void	setFont( ci::gl::TextureFontRef font );
	
	int				mCounter;
	float			mRadius;
protected:
	std::string		mWord;
	float			mWordPixelLength;
	
	static ci::gl::TextureFontRef	sFont;
};
