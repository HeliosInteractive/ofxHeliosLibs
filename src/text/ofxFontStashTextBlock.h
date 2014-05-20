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

	enum TEXT_ALIGNMENT
	{
		LEFT = 0 , 
		CENTER = 1 ,
		RIGHT = 2 
	};

	ofxFontStashTextBlock( )  ;
	~ofxFontStashTextBlock( )  
	{
		//delete font ; 
		font = NULL ; 
	}
	
	void setup( string fontPath , float lineSpacing , string text , float fontSize, float _x , float _y , ofColor color ) ;
	//void setup( ofxFontStash * fontStash , float lineSpacing , string text , float fontSize, float _x , float _y , ofColor color ) ;

	void draw( ) ;
	void draw( float x , float y ) ;
	void wrapTextX ( float maxX , float maxNumLines = -1 ) ; 

	string text ; 
	float x , y ; 
	int numLines ; 
	int maxNumLines ; 
	float columnWidth ; 
	float lineSpacing ; 
	ofColor color ; 
	

	ofRectangle boundingBox ; 
	ofxFontStash * font ;
	float fontSize ; 

	PARAGRAPH_FORMAT format ; 
	TEXT_ALIGNMENT alignment ; 

	float getWidth() { return boundingBox.getWidth() ; } 
	float getHeight() { return boundingBox.getHeight() ; }
	
};