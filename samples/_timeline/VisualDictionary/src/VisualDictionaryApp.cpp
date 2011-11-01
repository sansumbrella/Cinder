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


struct Circle {
  public:
	Circle( float radius, Color color )
		: mRadius( radius ), mColor( color )
	{}
	void draw( const Vec2f &pos ) {
		gl::color( ColorA( mColor * 0.25f, 0.25f ) );
		gl::drawSolidCircle( pos, mRadius() );
		gl::drawSolidCircle( pos + Vec2f( 0.5f, 0.5f ), mRadius );
		gl::drawSolidCircle( pos + Vec2f( 1.0f, 1.0f ), mRadius + 1 );
		
		gl::color( mColor );
		gl::drawSolidCircle( pos, mRadius() );
	}

	Anim<float>	mRadius;
	Color		mColor;
};


class VisualDictionaryApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void layoutWords( vector<string> words, float radius );	
	void setup();

	void enableSelections() { mEnableSelections = true; }
	void mouseMove( MouseEvent event );	
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	float getLayoutRadius(){ return getWindowHeight() * 0.415f; }

	void selectNode( list<WordNode>::iterator selectedWord );

	void update();
	void draw();

	list<WordNode>::iterator	getNodeAtPoint( const Vec2f &point );

	Anim<ColorA>				mBgColor;
	
	shared_ptr<Dictionary>		mDictionary;
	list<WordNode>				mNodes, mDyingNodes;
	list<WordNode>::iterator	mMouseOverNode;
	
	list<Circle>				mCircles;
	
	bool						mEnableSelections;
	WordNode					mCurrentNode;
	float						mCurrentCircleRadius;
};

void VisualDictionaryApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 800 );
}

void VisualDictionaryApp::layoutWords( vector<string> words, float radius )
{
	int radiusDivisor = 26;//std::max<int>( 10, words.size() ); // don't let the circles get too small
	mCurrentCircleRadius = radius / radiusDivisor * M_PI;
	for( size_t w = 0; w < words.size(); ++w ) {
		int wordLength = words[w].length();
		string s = words[w];
		int charIndex = (int)s[wordLength-1] - 97;
		float charPer = charIndex/26.0f;
		float angle = charPer * 2.0f * M_PI;
		//float angle = w / (float)words.size() * 2 * M_PI;
		Vec2f pos = getWindowCenter() + radius * Vec2f( cos( angle ), sin( angle ) );
		Color col(  CM_HSV, charPer, 1, 1 );
		mNodes.push_back( WordNode( words[w], false ) );
		mNodes.back().mPos = getWindowCenter();
		mNodes.back().mColor = ColorA( col, 0.0f );
		mNodes.back().mTextColor = ColorA( 0, 0, 0, 0.5f );
		
		timeline().apply( &mNodes.back().mRadius, mCurrentCircleRadius, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.5f );
		timeline().apply( &mNodes.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.475f );
		timeline().apply( &mNodes.back().mColor, ColorA( col, 1.0f ), 0.5f, EaseOutAtan( 10 ) ).timelineEnd( -0.475f );
	}
}

void VisualDictionaryApp::setup()
{
	mBgColor = ColorA( 0.0f, 0.0f, 0.2f, 1.0f );
	
	// load the dictionary
	mDictionary = shared_ptr<Dictionary>( new Dictionary( loadResource( "EnglishDictionary.gz" ) ) );

	// give the WordNodes their font
	WordNode::setFont( gl::TextureFont::create( Font( loadResource( "Ubuntu-M.ttf" ), 96 ), gl::TextureFont::Format().enableMipmapping( true ) ) );

	// make the first 26 nodes, one for each letter
	vector<string> initialWords;
	for( char c = 0; c < 26; ++c )
		initialWords.push_back( string( 1, (char)('a' + c) ) );

	layoutWords( initialWords, getLayoutRadius() );
	
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
	if( ! mEnableSelections )
		return;
	
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
	list<WordNode>::iterator clickedNode = getNodeAtPoint( event.getPos() );
	if( clickedNode != mNodes.end() )
		selectNode( clickedNode );
}


void VisualDictionaryApp::mouseMove( MouseEvent event )
{
	if( ! mEnableSelections )
		return;
	
	list<WordNode>::iterator currentMouseOver = getNodeAtPoint( event.getPos() );

	if( currentMouseOver != mMouseOverNode ) {
		mMouseOverNode = currentMouseOver;
		
		// make all the circles not moused-over small, and the mouse-over big
		for( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) {
			if( mMouseOverNode == nodeIt ){
				timeline().apply( &nodeIt->mRadius, mCurrentCircleRadius * 1.35f, 0.25f, EaseOutElastic( 200.0f, 120.0f ) );
				timeline().apply( &nodeIt->mTextColor, ColorA( 1, 1, 1, 1 ), 0.2f, EaseOutAtan( 10 ) );
			} else {
				timeline().apply( &nodeIt->mRadius, mCurrentCircleRadius, 0.5f, EaseOutAtan( 10 ) );
				timeline().apply( &nodeIt->mTextColor, ColorA( 0, 0, 0, 0.5f ), 0.2f, EaseOutAtan( 10 ) );
			}
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
	mCurrentNode.setIsSelected();

	for( list<Circle>::iterator circleIt = mCircles.begin(); circleIt != mCircles.end(); ++circleIt ){
		timeline().apply( &circleIt->mRadius, circleIt->mRadius + 10.0f, 0.75f, EaseInOutAtan( 10 ) );
	}
	mCircles.push_back( Circle( 140.0f, mCurrentNode.mColor() ) );
	

	mNodes.clear();

	// move the selected node to the center and make it big, transparent/white
	timeline().apply( &mCurrentNode.mRadius, 140.0f, 0.5f, EaseOutAtan( 10 ) );
	timeline().apply( &mCurrentNode.mPos, getWindowCenter(), 0.5f, EaseOutAtan( 10 ) );
	timeline().apply( &mCurrentNode.mTextColor, ColorA( 1, 1, 1, 1 ), 0.5f, EaseOutAtan( 10 ) );
	
	
	// now add all the descendants of the clicked node
	vector<string> children( mDictionary->getDescendants( mCurrentNode.getWord() ) );
	layoutWords( children, getLayoutRadius() );
	
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
	
	// draw the center circles
	int count = 0;
	if( ! mCurrentNode.getWord().empty() ){
		int wordLength = mCurrentNode.getWord().length();
		for( list<Circle>::iterator circleIt = mCircles.begin(); circleIt != mCircles.end(); ++circleIt ){
			if( count < wordLength-1 ){
				circleIt->draw( getWindowCenter() );
			}
			count ++;
		}
	}

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
}


CINDER_APP_BASIC( VisualDictionaryApp, RendererGl )
