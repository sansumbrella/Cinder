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
	void layoutWords( vector<string> words, Vec2f center, float radius );	
	void setup();

	void enableSelections() { mEnableSelections = true; }
	void mouseMove( MouseEvent event );	
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );

	void selectNode( list<WordNode>::iterator selectedWord );

	void update();
	void draw();

	list<WordNode>::iterator	getNodeAtPoint( const Vec2f &point );
	
	Anim<Vec2f>					mCenterPoint;
	Anim<ColorA>				mBgColor;
	
	shared_ptr<Dictionary>		mDictionary;
	list<WordNode>				mNodes, mDyingNodes;
	list<WordNode>::iterator	mMouseOverNode;
	bool						mEnableSelections;
	WordNode					mCurrentNode;
	float						mCurrentCircleRadius;
};

void VisualDictionaryApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1024, 1024 );
}

void VisualDictionaryApp::layoutWords( vector<string> words, Vec2f center, float radius )
{
	int radiusDivisor = std::max<int>( 10, words.size() ); // don't let the circles get too small
	mCurrentCircleRadius = radius / radiusDivisor * M_PI;
	for( size_t w = 0; w < words.size(); ++w ) {
		float angle = w / (float)words.size() * 2 * M_PI;
		Vec2f pos = center + radius * Vec2f( cos( angle ), sin( angle ) );
		Color col(  CM_HSV, w / (float)words.size(), 1, 1 );
		mNodes.push_back( WordNode( words[w], false ) );
		mNodes.back().mPos = center;
		mNodes.back().mColor = ColorA( col, 0.0f );
		
		timeline().apply( &mNodes.back().mRadius, mCurrentCircleRadius, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.5f );
		timeline().apply( &mNodes.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.475f );
		timeline().apply( &mNodes.back().mColor, ColorA( col, 1.0f ), 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.475f );
	}
}

void VisualDictionaryApp::setup()
{
	mCenterPoint = getWindowCenter();
	mBgColor = ColorA( 0.0f, 0.0f, 0.2f, 1.0f );
	
	// load the dictionary
	mDictionary = shared_ptr<Dictionary>( new Dictionary( loadResource( "EnglishDictionary.gz" ) ) );

	// give the WordNodes their font
	WordNode::setFont( gl::TextureFont::create( Font( loadResource( "Ubuntu-M.ttf" ), 96 ), gl::TextureFont::Format().enableMipmapping( true ) ) );

	// make the first 26 nodes, one for each letter
	vector<string> initialWords;
	for( char c = 0; c < 26; ++c )
		initialWords.push_back( string( 1, (char)('a' + c) ) );

	layoutWords( initialWords, mCenterPoint, getWindowHeight()*0.35f );
	
	// mark our currently highlighted node as "none"
	mMouseOverNode = mNodes.end();
	
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


void VisualDictionaryApp::keyDown( KeyEvent event )
{
	if( isalpha( event.getChar() ) ){
		// see if we can find a word that ends with this letter
		list<WordNode>::iterator foundWord = mNodes.end();
		for( foundWord = mNodes.begin(); foundWord != mNodes.end(); ++foundWord ) {
			if( foundWord->getWord()[foundWord->getWord().size()-1] == event.getChar() )
				break;
		}
		
		if( foundWord != mNodes.end() )
			selectNode( foundWord );
	} else {
		if( event.getCode() == KeyEvent::KEY_BACKSPACE ){
			
		}
	}
}

void VisualDictionaryApp::mouseDown( MouseEvent event )
{
	list<WordNode>::iterator clickedNode = getNodeAtPoint( event.getPos() - getWindowCenter() + (Vec2f)mCenterPoint );
	if( clickedNode != mNodes.end() )
		selectNode( clickedNode );
}


void VisualDictionaryApp::mouseMove( MouseEvent event )
{
	if( ! mEnableSelections )
		return;
	
	list<WordNode>::iterator currentMouseOver = getNodeAtPoint( event.getPos() - getWindowCenter() + (Vec2f)mCenterPoint );

	if( currentMouseOver != mMouseOverNode ) {
		mMouseOverNode = currentMouseOver;
		
		// make all the circles not moused-over small, and the mouse-over big
		for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
			if( mMouseOverNode == mNodes.end() )
				timeline().apply( &nodeIt->mRadius, mCurrentCircleRadius, 0.25f, EaseOutAtan( 10 ) );
			else if( mMouseOverNode == nodeIt )
				timeline().apply( &nodeIt->mRadius, mCurrentCircleRadius * 1.35f, 0.25f, EaseOutAtan( 10 ) );
			else
				timeline().apply( &nodeIt->mRadius, mCurrentCircleRadius, 0.5f, EaseOutAtan( 10 ) );
		}
	}
}

void VisualDictionaryApp::selectNode( list<WordNode>::iterator selectedNode )
{
	// have all the nodes but this one disappear
	for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
		if( nodeIt != selectedNode ) {
			// copy this node to dying nodes and erase it from the current
			mDyingNodes.push_back( *nodeIt );
			timeline().apply( &mDyingNodes.back().mRadius, 0.0f, 1.0f, EaseOutAtan( 10 ) )
				.completionFn( bind( &WordNode::setShouldBeDeleted, &(mDyingNodes.back()) ) ); // when you're done, mark yourself for deletion
		}
	}
	
	mCurrentNode = *selectedNode;

	mNodes.clear();


	// move the centerpoint to the selected node's position
	timeline().apply( &mCenterPoint, (Vec2f)mCurrentNode.mPos, 1.0f, EaseOutAtan( 10 ) );
	timeline().apply( &mBgColor, ColorA( mCurrentNode.mColor().r * 0.5f, mCurrentNode.mColor().g * 0.5f, mCurrentNode.mColor().b * 0.5f, 1.0f ), 1.0f, EaseOutAtan( 10 ) );
	
	// move the selected node to the center and make it big, transparent/white
	timeline().apply( &mCurrentNode.mRadius, 96.0f, 1.0f, EaseOutAtan( 10 ) );
//	timeline().apply( &mCurrentNode.mPos, mCenterPoint, 0.5f, EaseOutAtan( 10 ) );
	timeline().apply( &mCurrentNode.mColor, ColorA( 1, 1, 1, 0 ), 0.5f, EaseOutAtan( 10 ) );

	// now add all the descendants of the clicked node
	vector<string> children( mDictionary->getDescendants( mCurrentNode.getWord() ) );
	layoutWords( children, (Vec2f)mCurrentNode.mPos, getWindowHeight()*0.35f );
	
	// mark our currently highlighted node as "none"
	mMouseOverNode = mNodes.end();
	
	// once everything is done animating, then we can allow selections, but for now, disable them
	mEnableSelections = false;
	timeline().add( bind( &VisualDictionaryApp::enableSelections, this ), timeline().getEndTime() );
}

void VisualDictionaryApp::update()
{
	// erase any nodes which have been marked as ready to be deleted
	mDyingNodes.remove_if( bind( &WordNode::shouldBeDeleted, _1 ) );
}

void VisualDictionaryApp::draw()
{
	gl::clear( mBgColor ); 
	gl::enableAlphaBlending();
	
	gl::pushMatrices();
	gl::translate( getWindowCenter() - mCenterPoint );
	
	// draw the dying nodes
	for( list<WordNode>::const_iterator nodeIt = mDyingNodes.begin(); nodeIt != mDyingNodes.end(); ++nodeIt )
		nodeIt->draw();
	
	// draw all the non-mouseOver nodes
	for( list<WordNode>::const_iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ){
		if( nodeIt != mMouseOverNode )
			nodeIt->draw();
	}
	
	// if there is a mouseOverNode, draw it last so it is 'above' the others
	if( mMouseOverNode != mNodes.end() )
		mMouseOverNode->draw();
	
	// if there is a currentNode (previously selected), draw it	
	if( ! mCurrentNode.getWord().empty() )
		mCurrentNode.draw();
	
	gl::popMatrices();
}


CINDER_APP_BASIC( VisualDictionaryApp, RendererGl )
