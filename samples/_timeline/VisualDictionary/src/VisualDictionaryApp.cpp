#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"

#include <list>
#include <algorithm>

#include "WordNode.h"
#include "Dictionary.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VisualDictionaryApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void enableSelections() { mEnableSelections = true; }
	void mouseMove( MouseEvent event );	
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	list<WordNode>::iterator	getNodeAtPoint( const Vec2f &point );
	
	shared_ptr<Dictionary>		mDictionary;
	list<WordNode>				mNodes;
	list<WordNode>::iterator	mHighlightedNode;
	bool						mEnableSelections;
};

void VisualDictionaryApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 600 );
}

void VisualDictionaryApp::setup()
{
	// load the dictionary
	mDictionary = shared_ptr<Dictionary>( new Dictionary( loadResource( "EnglishDictionary.gz" ) ) );

	// give the WordNodes their font
	WordNode::setFont( gl::TextureFont::create( Font( loadResource( "Ubuntu-M.ttf" ), 72 ), gl::TextureFont::Format().enableMipmapping( true ) ) );

	// make the first 26 nodes, one for each letter
	for( char c = 0; c < 26; ++c ) {
		float angle = c / (float)26 * 2 * M_PI;
		Vec2f pos = getWindowCenter() + 250 * Vec2f( cos( angle ), sin( angle ) );
		std::string thisLetter( 1, (char)('A' + c) );
		mNodes.push_back( WordNode( thisLetter, false ) );
		mNodes.back().mColor = Color( CM_HSV, c / (float)26, 1, 1 );
		mNodes.back().mPos = getWindowCenter();
		timeline().apply( &mNodes.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.45f );
		timeline().apply( &mNodes.back().mRadius, 30.0f, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.5f );
	}
	
	// mark our currently highlighted node as "none"
	mHighlightedNode = mNodes.end();
	
	mEnableSelections = true;
}

list<WordNode>::iterator VisualDictionaryApp::getNodeAtPoint( const Vec2f &point )
{
	for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
		if( nodeIt->isPointInside( point ) )
			return nodeIt;
	}
	
	return mNodes.end();
}

void VisualDictionaryApp::mouseDown( MouseEvent event )
{
	list<WordNode>::iterator clickedNode = getNodeAtPoint( event.getPos() );
	if( clickedNode != mNodes.end()  ) {
		// have all the nodes but this one disappear
		for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
			if( nodeIt != clickedNode ) {
				timeline().apply( &nodeIt->mRadius, 0.0f, 1.0f, EaseOutAtan( 10 ) )
					.completionFn( bind( &WordNode::setShouldBeDeleted, &(*nodeIt) ) ); // when you're done, mark yourself for deletion
			}
		}
		
		// move the clicked node to the center and make it big
		timeline().apply( &clickedNode->mRadius, 60.0f, 1.0f, EaseOutAtan( 10 ) );
		timeline().apply( &clickedNode->mPos, getWindowCenter(), 0.5f, EaseOutAtan( 10 ) );
				
		// now add all the descendants of the clicked node
		vector<string> children( mDictionary->getDescendants( clickedNode->getWord() ) );
		for( size_t c = 0; c < children.size(); ++c ) {
			float angle = c / (float)children.size() * 2 * M_PI;
			Vec2f pos = getWindowCenter() + 250 * Vec2f( cos( angle ), sin( angle ) );
			mNodes.push_back( WordNode( children[c], false ) );
			mNodes.back().mColor = Color( CM_HSV, c / (float)children.size(), 1, 1 );
			mNodes.back().mPos = getWindowCenter();
			timeline().apply( &mNodes.back().mPos, pos, 0.25f, EaseOutAtan( 3 ) ).timelineEnd( -0.20f );
			timeline().apply( &mNodes.back().mRadius, 30.0f, 0.25f, EaseOutAtan( 3 ) ).timelineEnd( -0.25f );
		}
		
		// once everything is done animating, then we can allow selections, but for now, disable them
		mEnableSelections = false;
		timeline().add( bind( &VisualDictionaryApp::enableSelections, this ), timeline().getEndTime() );
	}
}

void VisualDictionaryApp::mouseMove( MouseEvent event )
{
	if( ! mEnableSelections )
		return;
	
	list<WordNode>::iterator currentMouseOver = getNodeAtPoint( event.getPos() );

	if( currentMouseOver != mHighlightedNode ) {
		mHighlightedNode = currentMouseOver;
		
		float smallRadius = ( mHighlightedNode != mNodes.end() ) ? 25 : 30;
		for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
			if( mHighlightedNode == nodeIt )
				timeline().apply( &nodeIt->mRadius, 50.0f, 0.25f, EaseOutAtan( 10 ) );
			else
				timeline().apply( &nodeIt->mRadius, smallRadius, 0.5f, EaseOutAtan( 10 ) );
		}
	}
}

void VisualDictionaryApp::update()
{
	// erase any nodes which have been marked as ready to be deleted
	mNodes.remove_if( bind( &WordNode::shouldBeDeleted, _1 ) );
}

void VisualDictionaryApp::draw()
{
	gl::clear( Color( 0.3f, 0.3f, 0.3f ) ); 
	gl::enableAlphaBlending( false );
	
	for( list<WordNode>::const_iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt )
		nodeIt->draw();
}


CINDER_APP_BASIC( VisualDictionaryApp, RendererGl )
