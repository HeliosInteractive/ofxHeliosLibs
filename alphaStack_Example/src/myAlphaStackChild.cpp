#include "myAlphaStackChild.h"

void myAlphaStackChild::setup ( float _x , float _y )
{
	x = _x ; 
	y = _y ; 
    color = ofColor ( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ) ;
    calcAlpha = 1.0f ;
    alpha = 1.0f ;
}

void myAlphaStackChild::draw( )
{
	ofPushStyle() ;
		ofEnableAlphaBlending() ; 
		ofSetRectMode( OF_RECTMODE_CORNER ) ; 
		ofSetColor( color.r , color.g , color.b , getOFAlpha() ) ;
		ofRect ( x , y  , 20 , 20 ) ;
	ofPopStyle() ; 
}

void myAlphaStackChild::update( )
{

}