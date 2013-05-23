#include "MultiTouchObject.h"

MultiTouchObject::MultiTouchObject()
{
    //ctor
}

MultiTouchObject::~MultiTouchObject()
{
    //dtor
}

void MultiTouchObject::setup ( float _x , float _y , float _size , float _dampen )
{
    scale = 1.0f ;
    rotation = 0.0f ;
    float size = _size ;
	x = _x ; 
	y = _y ; 
    area = ofRectangle (  -size/2 , -size/2 , size , size ) ;
	setHitBounds( area ) ;
    color = ofColor( ofRandom( 255 ) , ofRandom ( 255 ) , ofRandom( 255 ) ) ;
    multiTouch.setup( ) ;
    //Set min/max scales
    multiTouch.setScaleRange( .5f , 1.5f ) ;
    dampen = _dampen;
	

}

void MultiTouchObject::update ( )
{
    multiTouch.update( ) ;
	x += ( multiTouch.deltaX * dampen ) ;
    y += ( multiTouch.deltaY * dampen ) ;
    rotation += ( multiTouch.deltaDeg  * dampen ) ;
    scale = multiTouch.scaleX ;
}

void MultiTouchObject::draw ( )
{
	matrixPush() ; 
        ofSetColor( color ) ;
        ofRect( area ) ;
	matrixPop() ; 
}

string MultiTouchObject::getDebugOutput() 
{
	stringstream ss ; 
	ss << "x: " << x << " , y: " << y << endl ; 
	ss << "rot: " << rotation << " , scale : " << scale << endl ; 

	return ss.str() ; 
}


