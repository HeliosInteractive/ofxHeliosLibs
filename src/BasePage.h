#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"
#include "Tweenzor.h"

class BasePage : public ofxAlphaStackItem 
{
public : 
	BasePage( ) { } 
	~BasePage( ) { } 

	virtual void setup( string _label , int _id ) ;
	virtual void update ( ) ; 
	virtual void draw( ) ; 
	virtual void renderMap( ) ;
	virtual void debugDraw( ) ;

	virtual void transitionIn( ) ;
	virtual void transitionOut( ) ;  
	virtual void reset ( ) { } 

	virtual void input( float x , float y ) ; 
	string label ;
	int id ; 
	ofColor debugColor ;
};