
#include "ofxMatrixTransformObject.h"


void ofxMatrixTransformObject::setup ( )
{
    rotation = 0 ;
    scale = 1.0f ;
    x = 0 ;
    y = 0 ;
	z = 0 ; 
}
void ofxMatrixTransformObject::pushMatrix( )
{
    //translate -> rotate -> scale
    ofPushMatrix ( ) ;
    ofTranslate ( x , y , 0 ) ;
    ofScale ( scale , scale , 1.0f );
    ofRotateZ ( rotation ) ;
}

void ofxMatrixTransformObject::popMatrix ( )
{
    ofPopMatrix( ) ;
}

void ofxMatrixTransformObject::setHitBounds ( ofRectangle _rect )
{
    hitBounds = _rect ;
}

ofRectangle ofxMatrixTransformObject::getHitBounds ( )
{
    return hitBounds ;
}

/* does not work on rotated objects... YET! */
bool ofxMatrixTransformObject::hitTest( ofVec2f p , ofRectangle _bounds )
{
    return (( p.x > _bounds.x ) && ( p.x < _bounds.x + _bounds.width) && ( p.y > _bounds.y ) && ( p.y  < _bounds.y + _bounds.height ));
}

bool ofxMatrixTransformObject::hitTest( ofVec2f p )
{
	bool bResult = hitTest( p , getHitBounds() ) ;
	return bResult ;
//    return (( p.x > _bounds.x ) && ( p.x < _bounds.x + _bounds.width) && ( p.y > _bounds.y ) && ( p.y  < _bounds.y + _bounds.height ));
}


