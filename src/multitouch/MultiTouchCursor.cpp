#include "MultiTouchCursor.h"

MultiTouchCursor::MultiTouchCursor()
{
    //ctor
}

MultiTouchCursor::~MultiTouchCursor()
{
    //dtor
}

void MultiTouchCursor::setup ( int _id  , ofPoint p  ) 
{
	setup( _id , p.x , p.y , p.z ) ;
}

void MultiTouchCursor::setup (  int _id ,  float _x , float _y , float _z )
{
    id = _id ;
	position = ofPoint( _x , _y , _z ) ; 
    bDrag = false;
}

void MultiTouchCursor::update ( )
{

}

void MultiTouchCursor::draw ( )
{
    ofPushMatrix( ) ;
        ofTranslate( position.x * ofGetWidth() , position.y * ofGetHeight() ) ;
        ofPushStyle() ;
        ofNoFill( ) ;
        if ( bDrag )
            ofFill( ) ;
        ofSetColor( 0 , 255 , 0 ) ;
        ofCircle( 0 , 0, 15  ) ;
        string status = "id:" + ofToString(id) + " x:" + ofToString( (int)position.x ) + " y:" + ofToString( (int)position.y ) ;
        ofSetColor( 255 , 255 , 255 ) ;
        ofDrawBitmapString( status , 0 , 20 ) ;
        ofPopStyle( ) ;
    ofPopMatrix( ) ;
}

void MultiTouchCursor::onDown ( ofPoint p )
{
    bDrag = true ;
    position = p ;
}
void MultiTouchCursor::onDrag( ofPoint p )
{
    position = p ;
}

void MultiTouchCursor::onUp ( ofPoint p )
{
    bDrag = false ;
    position = p ;
}
