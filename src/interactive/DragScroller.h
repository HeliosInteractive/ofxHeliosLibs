#pragma once

#include "ofMain.h"
#include "Tweenzor.h"
#include "BaseMultitouchObject.h"

class DragScroller : public BaseMultitouchObject
{
public : 
	DragScroller( ) { } 
	~DragScroller( ) { } 

	void setup(  ofRectangle area , float overscroll = 0.0f , float dragSensitivity = 1.0f , float dragInterpolationTime = 0.1f  ) ; 
	void update( ) ; 
	void draw( ) ; 
	void reset( ) ; 
	void inputDown( float x , float y , int id ) ;
	void inputDrag( float x , float y , int id ) ;
	void inputUp( float x , float y , int id ) ;
	
	float limitDragValue( float input ,float min, float max ,  bool bEnableOverscroll ) ; 
	float overscroll ; 
	ofPoint originDrag ; 
	float dragSensitivity ; 
	bool bDragging ; 
	ofRectangle dragArea ; 
	float minY , maxY , scrollY , startScrollY ; 
	float minX , maxX , scrollX , startScrollX ; 
	float dragInterpolationTime ; 
	
	void setupScrollVertical( float minY , float maxY ) ; 
	void setupScrollHorizontal( float minX , float maxX ) ; 
	stringstream debug ; 

private :
	bool bScrollVertical ; 
	bool bScrollHorizontal ; 
};