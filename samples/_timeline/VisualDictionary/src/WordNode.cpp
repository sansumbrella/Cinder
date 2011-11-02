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

gl::TextureFontRef	WordNode::sFontSmall;
gl::TextureFontRef	WordNode::sFontBig;

WordNode::WordNode( const string &word )
	: mWord( word ), mShouldBeDeleted( false ), mIsSelected( false )
{
	mWordPixelLengthSmall	= sFontSmall->measureString( mWord ).x;
	mWordPixelLengthBig		= sFontBig->measureString( mWord ).x;
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

void WordNode::setSelected()
{
	mIsSelected = true;
}

bool WordNode::isSelected() const
{
	return mIsSelected;
}

void WordNode::draw() const
{
	// draw shadows
	gl::color( ColorA( 0, 0, 0, mColor().a * 0.5f ) );
	Vec2f p	= mPos();
	p += Vec2f( 1.0f, 1.0f );
	gl::drawSolidRect( Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );
	p += Vec2f( 1.0f, 1.0f );
	gl::drawSolidRect( Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );

	
	// draw color circle
	gl::color( mColor );
	p = mPos();
	gl::drawSolidRect( Rectf( p.x - mRadius, p.y - mRadius, p.x + mRadius, p.y + mRadius ) );

	
	// draw string
	// biggest square that can fit in the circle is radius * sqrt(2) per side  x^2 = (r^2)/2
	const float squareSide = sqrtf( ( mRadius * mRadius ) / 2.0f );
	
	// this is ugly. 
	if( mIsSelected ){
		float pixelScale = std::min( squareSide / mWordPixelLengthBig, squareSide / 150 ) * 2.0f;
		gl::TextureFont::DrawOptions options = gl::TextureFont::DrawOptions().scale( pixelScale ).pixelSnap( false );
		
		const Vec2f offset = (Vec2f)mPos + Vec2f( -mRadius + ( mRadius * 2 - mWordPixelLengthBig * pixelScale ) / 2, mRadius - (mRadius * 2.0f - 60 * pixelScale ) / 2 );
		
		gl::color( ColorA( Color::black(), 0.5f ) );
		sFontBig->drawString( mWord, offset + Vec2f( pixelScale, pixelScale ) * 1.5f, options );
		
		gl::color( ColorA( Color::white(), 1.0f ) );
		sFontBig->drawString( mWord, offset, options );
		
	} else {
		float pixelScale = std::min( squareSide / mWordPixelLengthSmall, squareSide / 72 ) * 2.25f;
		gl::TextureFont::DrawOptions options = gl::TextureFont::DrawOptions().scale( pixelScale ).pixelSnap( false );

		const Vec2f offset = (Vec2f)mPos + Vec2f( -mRadius + ( mRadius * 2 - mWordPixelLengthSmall * pixelScale ) / 2, mRadius - (mRadius * 2.0f - 20 * pixelScale ) / 2 );

		gl::color( ColorA( Color::black(), 0.5f ) );
		sFontSmall->drawString( mWord, offset + Vec2f( pixelScale, pixelScale ) * 1.5f, options );

		gl::color( ColorA( ( mColor() + Color::white() ) * 0.65f, 1.0f ) );
		sFontSmall->drawString( mWord, offset, options );
	}
}

void WordNode::setFont( gl::TextureFontRef fontSmall, gl::TextureFontRef fontBig )
{
	sFontSmall = fontSmall;
	sFontBig   = fontBig;
}
