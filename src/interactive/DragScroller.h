#pragma once

#include "ofMain.h"
#include "Tweenzor.h"

class DragScroller
{
public : 
	DragScroller( ) { } 
	~DragScroller( ) { } 

	void setup(  ofRectangle area , float _minY , float _maxY , float overscroll = 0.0f , float dragSensitivity = 1.0f , float dragInterpolationTime = 0.1f  ) ; 
	void update( ) ; 
	void draw( ) ; 
	void reset( ) ; 
	void inputDown( float x , float y ) ;
	void inputDrag( float x , float y ) ;
	void inputRelease( float x , float y ) ;
	
	float limitDragValue( float input , bool bEnableOverscroll ) ; 
	float overscroll ; 
	ofPoint originDrag ; 
	float dragSensitivity ; 
	bool bDragging ; 
	ofRectangle dragArea ; 
	float minY , maxY , scrollY , startScrollY ; 
	float dragInterpolationTime ; 

	string debug ; 
};