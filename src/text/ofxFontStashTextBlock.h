#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"
#include "ofxAlphaStackItem.h"
#include "ofxFontStashManager.h"

class ofxFontStashTextBlock : public ofxAlphaStackItem
{
public : 
	enum PARAGRAPH_FORMAT
	{
		SINGLE_LINE = 0 , 
		MULTI_LINE = 1 ,
		MULTI_LINE_WIDTH_LIMIT = 2
	};

	ofxFontStashTextBlock( )  ; 
	
	void setup( string fontPath , float lineSpacing , string text , float fontSize, float _x , float _y , ofColor color ) ;
	//void setup( ofxFontStash * fontStash , float lineSpacing , string text , float fontSize, float _x , float _y , ofColor color ) ;

	void draw( ) ; 
	void wrapTextX ( float maxX , float maxNumLines = -1 ) ; 

	string text ; 
	float x , y ; 
	int numLines ; 
	int maxNumLines ; 
	float columnWidth ; 
	ofColor color ; 

	ofRectangle boundingBox ; 
	ofxFontStash * font ;
	float fontSize ; 

	PARAGRAPH_FORMAT format ; 
	
};