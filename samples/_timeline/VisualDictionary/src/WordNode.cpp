//
//  WordNode.cpp
//  VisualDictionary
//
//  Created by Andrew Bell on 10/28/11.
//  Copyright (c) 2011. All rights reserved.
//

#include "WordNode.h"
#include "cinder/gl/gl.h"

#include <iostream>

using namespace std;
using namespace ci;

gl::TextureFontRef	WordNode::sFont;

WordNode::WordNode( const string &word, bool completeWord )
	: mWord( word ), mCompleteWord( completeWord ), mShouldBeDeleted( false )
{
	mWordPixelLength = sFont->measureString( mWord ).x;
}

bool WordNode::isPointInside( const Vec2f &pt ) const
{
	return pt.distance( mPos ) < mRadius;
}

void WordNode::setShouldBeDeleted()
{
	mShouldBeDeleted = true;
}

bool WordNode::shouldBeDeleted() const
{
	return mShouldBeDeleted;
}

void WordNode::draw() const
{
	gl::color( mColor() * 0.3f );
	gl::drawSolidCircle( mPos, mRadius );

	
	gl::color( mColor );
	gl::drawSolidCircle( mPos, mRadius - 3.0f );

	// biggest square that can fit in the circle is radius * sqrt(2) per side  x^2 = (r^2)/2
	const float squareSide = sqrtf( mRadius * mRadius / 2.0f ) * 2;
	float pixelScale = std::min( squareSide / mWordPixelLength, squareSide / 72 );
	gl::TextureFont::DrawOptions options = gl::TextureFont::DrawOptions().scale( pixelScale ).pixelSnap( false );

	const Vec2f offset = (Vec2f)mPos + Vec2f( -mRadius + ( mRadius * 2 - mWordPixelLength * pixelScale ) / 2, mRadius - (mRadius * 2.0f - 60 * pixelScale ) / 2 );

	gl::color( ColorA( Color::black(), 0.5f ) );
	sFont->drawString( mWord, offset + Vec2f( pixelScale, pixelScale ) * 2.0f, options );
	gl::color( Color::white() );
	sFont->drawString( mWord, offset, options );
}

void WordNode::setFont( gl::TextureFontRef font )
{
	sFont = font;
}