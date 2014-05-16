#include "ofxFontStashTextBlock.h"

ofxFontStashTextBlock::ofxFontStashTextBlock( ) 
{
	text = "no text yet" ;
	x = 0 ; 
	y = 0 ; 
	numLines = 1 ; 
	maxNumLines = 50 ; 
	columnWidth = 100 ; 
	format = SINGLE_LINE ; 
	font = NULL ; 
}

void ofxFontStashTextBlock::setup( string fontPath , float lineSpacing , string _text , float _fontSize ,  float _x , float _y , ofColor _color = ofColor::white ) 
{
	font = ofxFontStashManager::Instance()->getFont( fontPath ) ; 
	//font.setup( fontPath , lineSpacing ) ;
	x = _x ; 
	y = _y ; 
	text = _text ; 
	fontSize = _fontSize ; 
	format = SINGLE_LINE ; 
	color = _color ; 
	ofxAlphaStackItem::setup( ) ; 
}


void ofxFontStashTextBlock::draw( ) 
{
	if ( font == NULL  ) return ; 

	ofPushStyle() ; 
	ofSetColor( color , getOFAlpha() ) ; 
	switch ( format ) 
	{
		case SINGLE_LINE:
			font->draw( text , fontSize , x , y ) ;  
			boundingBox = font->getBBox( text , fontSize, x , y ) ; 
			break ; 

		case MULTI_LINE :
			font->drawMultiLine( text , fontSize, x , y ) ; 
			boundingBox = font->getBBox( text , fontSize, x , y ) ; 
			break ; 

		case MULTI_LINE_WIDTH_LIMIT : 
			boundingBox = font->drawMultiLineColumn( text , fontSize , x , y , columnWidth , numLines , false , maxNumLines , false ) ; 
			break ;
	}
	ofPopStyle() ; 
	//font.drawMultiLineColumn( text , fontSize , x , y , 
}

void ofxFontStashTextBlock::wrapTextX( float maxX , float _maxNumLines ) 
{
	columnWidth = maxX ; 
	maxNumLines = _maxNumLines ; 
	format = MULTI_LINE_WIDTH_LIMIT ; 
}

/*
	void setup( string fontPath , float lineSpacing , float _x , float _y ) ; 
	void draw( ) ; 
	void wrapTextX ( float maxX , float maxNumLines = -1 ) ; 
*/