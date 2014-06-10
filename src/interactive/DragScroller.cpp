#include "DragScroller.h"


void DragScroller::setup( ofRectangle area , float _overscroll,  float _dragSensitivity  , float _dragInterpolationTime)
{
	dragArea = area ; 
	overscroll = _overscroll ; 
	dragInterpolationTime = _dragInterpolationTime ; 
	dragSensitivity = _dragSensitivity ; 
	reset( ) ; 
	bScrollHorizontal = false ; 
	bScrollVertical = false ; 
}

void DragScroller::setupScrollVertical( float _minY , float _maxY ) 
{
	minY = _minY ; 
	maxY = _maxY ; 
	scrollY = _minY ;
	bScrollVertical = true ; 
	ofLogNotice( "DragScroller::setupScrollVertical" ) << "range : " << minY << " <-> " << maxY ; 
}

void DragScroller::setupScrollHorizontal( float _minX , float _maxX ) 
{
	minX = _minX ; 
	maxX = _maxX ;
	scrollX = _minX ; 
	bScrollHorizontal = true ; 
	ofLogNotice( "DragScroller::setupScrollHorizontal" ) << "range : " << minX << " <-> " << maxX ; 
}

void DragScroller::reset( ) 
{
	bDragging = false ; 
	if ( bScrollHorizontal ) 
	{
		scrollX = maxX ; 
		startScrollX = scrollX ; 
		scrollY = dragArea.y ; 
	}
	if ( bScrollVertical ) 
	{
		scrollY = maxY ;
		startScrollY = scrollY ; 
		scrollX = dragArea.x ; 
	}
}

void DragScroller::update( ) 
{

}

void DragScroller::draw( ) 
{
	//Debug Drag
	//Drag hue colors down 20 segments to show scrolling area 
	if ( bScrollVertical ) 
	{
		ofPushMatrix() ; 
			ofTranslate( 0 , scrollY ) ; // scrollX , scrollY ) ; 
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
	}
	else if ( bScrollHorizontal ) 
	{
		ofPushMatrix() ; 
		//cout << " scrollX : " << scrollX << " - scrollY " << scrollY << " min: " << minX << " - max :" << maxX << endl ;  
			ofTranslate( scrollX , 0 ) ; // scrollX , scrollY ) ; 
			float numSegments = 20.0f ; 
			float perSegmentWidth = abs( maxX - minX ) / numSegments ; 
			for ( int i = 0 ; i < numSegments ; i++ ) 
			{
				float hue = ((float) i )/numSegments * 255.0f ; 
				ofColor c = ofColor::fromHsb( hue , 215.0f , 255.0f ) ; 
				ofSetColor( c ) ; 
				ofRect( dragArea.x + perSegmentWidth * i , dragArea.y , perSegmentWidth , dragArea.height ) ; 
			}
		ofPopMatrix() ; 
	}
	

	ofDrawBitmapStringHighlight( debug.str() , 25 , 25 ) ; 
}

void DragScroller::inputDown( float x , float y , int id ) 
{
	if ( dragArea.inside( x , y ) == true ) 
	{
		bDragging = true ; 
		originDrag = ofPoint ( x , y ) ; 
		startScrollY = scrollY ; 
		startScrollX = scrollX ; 
	}


}

void DragScroller::inputDrag( float x , float y , int id ) 
{
	if ( bDragging == true ) 
	{
		if ( bScrollVertical ) 
		{
			float yDiff = ( y - originDrag.y ) * dragSensitivity  ; 
			float newScrollY = limitDragValue( yDiff + startScrollY , minY , maxY , true ) ;
			Tweenzor::add( &scrollY , scrollY ,newScrollY , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
		}
		if ( bScrollHorizontal ) 
		{
			float xDiff = ( x - originDrag.x ) * dragSensitivity  ; 
			float newScrollX = limitDragValue( xDiff + startScrollX , minX , maxX , true ) ; 
			Tweenzor::add( &scrollX , scrollX ,newScrollX , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
		}
	}
}



void DragScroller::inputUp( float x , float y , int id ) 
{
	if ( bDragging == true )
	{
		//Add flick in here ? 
		//float yDiff = ( y - originDrag.y ) * dragSensitivity  ;
		//float newScrollY = limitDragValue( yDiff + startScrollY , false ) ; 
		//Tweenzor::add( &scrollY , scrollY ,newScrollY , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
		if ( bScrollVertical ) 
		{
			float yDiff = ( y - originDrag.y ) * dragSensitivity  ; 
			float newScrollY = limitDragValue( yDiff + startScrollY , minY , maxY , false ) ;
			Tweenzor::add( &scrollY , scrollY ,newScrollY , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
		}
		if ( bScrollHorizontal ) 
		{
			float xDiff = ( x - originDrag.x ) * dragSensitivity  ; 
			float newScrollX = limitDragValue( xDiff + startScrollX , minX , maxX , false ) ; 
			Tweenzor::add( &scrollX , scrollX ,newScrollX , 0.0f , dragInterpolationTime , EASE_OUT_SINE ) ; 
		}
	}
	bDragging = false ;
}

float DragScroller::limitDragValue( float input , float min, float max , bool bEnableOverscroll )
{
	float val = 0 ; 
	if ( bEnableOverscroll == true ) 
		val = overscroll ; 

	if ( input < ( min - val )  ) 
		input = ( min - val ) ; 
	if ( input > ( max + val ) ) 
		input =  ( max + val );
	return input ; 
}