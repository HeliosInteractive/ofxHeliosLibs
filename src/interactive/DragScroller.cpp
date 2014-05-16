#include "DragScroller.h"


void DragScroller::setup( ofRectangle area , float _minY , float _maxY , float _overscroll,  float _dragSensitivity  , float _dragInterpolationTime)
{
	dragArea = area ; 
	minY = _minY ; 
	maxY = _maxY ; 
	overscroll = _overscroll ; 
	dragInterpolationTime = _dragInterpolationTime ; 
	dragSensitivity = _dragSensitivity ; 
	reset( ) ; 
}

void DragScroller::reset( ) 
{
	bDragging = false ; 
	scrollY = maxY ;
	startScrollY = scrollY ; 
}

void DragScroller::update( ) 
{

}

void DragScroller::draw( ) 
{
	
	//Debug Drag
	//Drag hue colors down 20 segments to show scrolling area 
	ofPushMatrix() ; 
		ofTranslate( 0 , scrollY ) ; 
		float numSegments = 20.0f ; 
		float perSegmentHeight = abs( maxY - minY ) / numSegments ; 
		for ( int i = 0 ; i < numSegments ; i++ ) 
		{
			float hue = ((float) i )/numSegments * 255.0f ; 
			ofColor c = ofColor::fromHsb( hue , 215.0f , 255.0f ) ; 
			ofSetColor( c ) ; 
			ofRect( dragArea.x , dragArea.y + perSegmentHeight * i , dragArea.width , perSegmentHeight ) ; 
		}
	ofPopMatrix() ; 

	ofDrawBitmapStringHighlight( debug , 25 , 25 ) ; 
}

void DragScroller::inputDown( float x , float y ) 
{
	if ( dragArea.inside( x , y ) == true ) 
	{
		bDragging = true ; 
		originDrag = ofPoint ( x , y ) ; 
		startScrollY = scrollY ; 
	}
}

void DragScroller::inputDrag( float x , float y ) 
{
	if ( bDragging == true ) 
	{
		float yDiff = ( y - originDrag.y ) * dragSensitivity  ; 
		float newScrollY = limitDragValue( yDiff + startScrollY , true ) ; 
		//scrollY = newScrollY ; 
		Tweenzor::add( &scrollY , scrollY ,newScrollY , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
	}
}

float DragScroller::limitDragValue( float input , bool bEnableOverscroll )
{
	float val = 0 ; 
	if ( bEnableOverscroll == true ) 
		val = overscroll ; 

	if ( input < ( minY - val )  ) 
		input = ( minY - val ) ; 
	if ( input > ( maxY + val ) ) 
		input =  ( maxY + val );
	return input ; 
}

void DragScroller::inputRelease( float x , float y ) 
{
	if ( bDragging == true )
	{
		//Add flick in here ? 
		float yDiff = ( y - originDrag.y ) * dragSensitivity  ;
		float newScrollY = limitDragValue( yDiff + startScrollY , false ) ; 
		Tweenzor::add( &scrollY , scrollY ,newScrollY , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
	}
	bDragging = false ;
}