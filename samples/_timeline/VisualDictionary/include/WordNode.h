//
//  WordNode.h
//  VisualDictionary
//
//  Created by Andrew Bell on 10/28/11.
//  Copyright (c) 2011. All rights reserved.
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/TextureFont.h"

#include <string>

class WordNode {
  public:
	WordNode() {}
	WordNode( const std::string &word, bool completeWord );

	std::string		getWord() const { return mWord; }
	
	bool	shouldBeDeleted() const;
	void	setShouldBeDeleted();
	
	bool	isPointInside( const ci::Vec2f &pt ) const;
	
	void	draw() const;
		
	ci::Anim<ci::Vec2f>		mPos;
	ci::Anim<float>			mRadius;
	ci::Anim<ci::ColorA>	mColor;	


	static	void	setFont( ci::gl::TextureFontRef font );
  protected:
	bool			mShouldBeDeleted;
	std::string		mWord;
	bool			mCompleteWord;
	float			mWordPixelLength;
	
	static ci::gl::TextureFontRef	sFont;
};
