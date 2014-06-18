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
    color = ofColor( 60 ) ;
    multiTouch.setup( ) ;
    //Set min/max scales
    multiTouch.setScaleRange( 0.75f , 3.5f ) ;
    dampen = _dampen;
	

}

void MultiTouchObject::update ( )
{
    multiTouch.update( ) ;
	x += ( multiTouch.deltaX * dampen ) ;
    y += ( multiTouch.deltaY * dampen ) ;
	//z += ( multiTouch.delta.z * dampen ) ;
    rotation += ( multiTouch.deltaDeg  * dampen ) ;
	scale = multiTouch.scaleX ;
}

void MultiTouchObject::draw ( )
{
	pushMatrix() ; 
        ofSetColor( color ) ;
        ofRect( area ) ;
	popMatrix() ; 
}

string MultiTouchObject::getDebugOutput() 
{
	stringstream ss ; 
	ss << "x: " << x << " , y: " << y << endl ; 
	ss << "rot: " << rotation << " , scale : " << scale << endl ; 

	return ss.str() ; 
}


