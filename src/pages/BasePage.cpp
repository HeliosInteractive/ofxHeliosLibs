#include "BasePage.h"


void BasePage::setup( string _label , int _id ) 
{
	label = _label ; 
	id = _id ; 
	ofxAlphaStackItem::setup( ) ; 
	alpha = 0 ; 
	debugColor = ofColor::fromHsb( ofRandom ( 255 ) , 255 , 255 ) ; 
}

void BasePage::update ( ) 
{
	for ( auto child = children.begin() ; child != children.end() ; child++ ) 
		(*child)->alphaStackUpdate( calcAlpha ) ; 
}  

void BasePage::draw( ) 
{

}  

void BasePage::renderMap( ) 
{

}

void BasePage::debugDraw( )
{
	if ( alpha > 0 ) 
	{
		ofDrawBitmapStringHighlight( "PAGE : " + label , 15 , 15 ) ;
	}
}

void BasePage::transitionIn( ) 
{
	if ( alpha == 0 ) 
	{
		Tweenzor::add( &alpha , 0.0f , 1.0f , 0.0f , 0.65f , EASE_OUT_QUAD ) ; 
	}
}

void BasePage::transitionOut( ) 
{
	if ( alpha > 0 ) 
	{
		Tweenzor::add( &alpha , alpha , 0.0f , 0.0f , 0.65f , EASE_OUT_QUAD ) ; 
	}
}

void BasePage::inputDown( float x , float y ) 
{

}

void BasePage::inputDrag( float x , float y ) 
{

}

void BasePage::inputRelease( float x , float y ) 
{

} 