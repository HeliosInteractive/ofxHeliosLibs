#pragma once

#include "ofxMatrixTransformImage.h"
#include "Tweenzor.h"

class ofxImageButton 
{
public : 
	ofxImageButton ( ) { } 
	~ofxImageButton ( ) { } 

	virtual void setup( ) ;
	virtual void update( ) ; 
	virtual void loadImage( string spritePath , float _x , float _y , ofPoint anchor , float _alpha = 0.0f , string hitSpritePath = "" );
	virtual void draw( ) ;
	virtual void onHit( ) ;
	virtual void onReset( ) ; 
	virtual void setBoundsPadding( float ratio ) ; 
	virtual bool hitTestPoint ( ofPoint p ) ;
	virtual bool hitTestPoint ( float x , float y ) { return ( hitTestPoint ( ofPoint ( x , y ) ) ); } 

	float tweenHitArgs ; 
	virtual void hitStageComplete( float * args ) ; 
	virtual void hitComplete( float * args ) ; 

	bool bHit ; 
	ofPoint anchor ; 

	ofxMatrixTransformImage hitSprite; 
	ofxMatrixTransformImage sprite ; 
};
