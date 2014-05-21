#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetLogLevel( OF_LOG_VERBOSE ) ; 

	//unicodeFont.setup("Arial Unicode.ttf", 1.0f ); //load verdana font, set lineHeight to be 130%
	string text = (string)"ofxFontStash can draw multiline text" + "\n" +
	"It also supports unicode strings: " + "\n" +
	"槊監しゅ祟䤂לרפובליקה. אם מיזם 銆銌 憉 圩芰敔 तकनिकल कार्यलय";
	
	textBlock1.setup( "type/Whitney-Book-Bas.ttf" , 1.0f ,text , 28.0f , 50 , 50 , ofColor::white ) ; 
	textBlock1.wrapTextX( 300 ) ;

	textBlock2.setup( "Arial Unicode.ttf" , 1.0f , text , 28.0f , 50 , 350 , ofColor::white ) ; 
	textBlock2.wrapTextX( 800 ) ;
}


//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground( 128 ) ; 
	textBlock1.alpha = sin ( ofGetElapsedTimef() ) * 0.5f + 0.5f ;
	textBlock1.alphaStackUpdate( 1.0f ) ; 
	textBlock1.draw( ) ; 

	textBlock2.alphaStackUpdate( 1.0f ) ; 
	textBlock2.draw( ) ; 
}