#include "ofxSimpleImageButton.h"

ofxSimpleImageButton::ofxSimpleImageButton( ) 
{
	hitImage.alpha = 0.0f ; 
}

ofxSimpleImageButton::~ofxSimpleImageButton( ) 
{

}

void ofxSimpleImageButton::alphaStackUpdate( float _alpha ) 
{
	ofxMatrixTransformImage::alphaStackUpdate( _alpha ) ; 
	hitImage.alphaStackUpdate( _alpha ) ; 
}

void ofxSimpleImageButton::draw( ) 
{
	ofxMatrixTransformImage::draw( ) ; 
	hitImage.draw( ) ;
}

void ofxSimpleImageButton::hit( ) 
{
	Tweenzor::add( &hitImage.alpha , 1.0f, 0.0f , 0.0f , 0.12f , EASE_OUT_QUAD ) ; 
}

ofRectangle ofxSimpleImageButton::getScreenRect( ) 
{
	float _x = x + -_anchor.x * image.getWidth() ; 
	float _y = y + -_anchor.y * image.getHeight() ; 

	return ofRectangle( _x , _y , image.getWidth() , image.getHeight() ) ; 
}

bool ofxSimpleImageButton::hitTest( float x , float y ) 
{
	return ( getScreenRect().inside( x , y ) ) ; 
}