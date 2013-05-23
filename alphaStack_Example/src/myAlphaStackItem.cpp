#include "myAlphaStackItem.h"

void myAlphaStackItem::setup (  float _x , float _y , int numChildren )
{
	x = _x ; 
	y = _y ; 
    color = ofColor ( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ) ;
    calcAlpha = 1.0f ;
    alpha = 1.0f ;

    for ( int i = 0 ; i < numChildren ; i++ )
    {
        myAlphaStackChild child ;
		child.setup( _x + ofRandom( -50 , 50 ) , _y + ofRandom( -50 , 50 ) ) ; 
		children.push_back( child ) ; 
	}

    ofPopMatrix( ) ;
}

void myAlphaStackItem::update ( ) 
{
	for ( int i = 0 ; i < children.size() ; i++ )
    {
		children[i].alphaStackUpdate( calcAlpha ) ; 
		children[i].update( ) ; 
    }
}

void myAlphaStackItem::draw ( ) 
{
	ofPushStyle() ;
		ofSetRectMode( OF_RECTMODE_CORNER ) ;
		ofSetColor( color , getOFAlpha() ) ; 
		ofRect( x , y , 100 , 100 ) ;
		for ( int i = 0 ; i < children.size() ; i++ )
		{
			children[i].draw ( ) ;
		}
	ofPopStyle() ;
}