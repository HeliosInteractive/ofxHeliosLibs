/*

insert license here

*/


#pragma once

#include "ofMain.h"
#include "ofxMatrixTransformImage.h"
#include "ofxTweenzor.h"

class ofxSimpleImageButton : public ofxMatrixTransformImage 
{
public : 
	ofxSimpleImageButton( ) ; 
	~ofxSimpleImageButton( ) ; 

	void draw( ) ; 
	void alphaStackUpdate( float _alpha ) ;
	void hit( ) ; 

	bool hitTest( float x , float y ) ; 
	ofRectangle getScreenRect( ) ; 

	ofxMatrixTransformImage hitImage ; 
};