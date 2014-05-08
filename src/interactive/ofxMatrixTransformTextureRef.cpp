#include "ofxMatrixTransformTextureRef.h"


ofxMatrixTransformTextureRef::ofxMatrixTransformTextureRef ( )
{
	texture= NULL ; 
	scale = 1.0f ;
	rotation = 0.0f ;
	bInheritAlpha = false ;
	setup( ) ; 
	//alpha = 0 ;
}
    
ofxMatrixTransformTextureRef::~ofxMatrixTransformTextureRef ( )
{
	texture = NULL ; 
    //image->clear() ; 
}

void ofxMatrixTransformTextureRef::setup ( ) 
{
	ofxMatrixTransformObject::setup() ;
	ofxAlphaStackItem::setup ( ) ; 

	scale = 1.0f ;
	rotation = 0.0f ;
	bInheritAlpha = false ;
}

void ofxMatrixTransformTextureRef::draw ( )
{
	pushMatrix( ) ;
        ofSetColor ( 255 , 255 , 255 , getOFAlpha() ) ;
		if ( texture != NULL && texture->bAllocated() ) 
			texture->draw ( texture->getWidth() * -_anchor.x , texture->getHeight() * -_anchor.y ) ;
		//ofSetColor( 255 , 0 , 0 , getOFAlpha() ) ; 
		//ofRect( texture->getWidth() * -_anchor.x , texture->getHeight() * -_anchor.y , texture->getWidth() , texture->getHeight() ) ; 
	popMatrix( ) ;
}


void ofxMatrixTransformTextureRef::setImageProperties( string path , float _x , float _y , ofPoint anchor , float _alpha  )
{
	texture = new ofImage( ); 
	texture->loadImage( "path" ) ; 
	//cout << " texture is now : " << texture->getWidth() << " x " << texture->getHeight() << endl ; 
    x = _x ;
    y = _y ;
    _anchor = anchor ;
    alpha = _alpha ;
}


void ofxMatrixTransformTextureRef::setImageProperties( ofImage * image , float _x , float _y , ofPoint anchor , float _alpha  )
{
	texture = image ; 
	//texture->loadImage( "path" ) ; 
	//cout << " texture is now : " << texture->getWidth() << " x " << texture->getHeight() << endl ; 
    x = _x ;
    y = _y ;
    _anchor = anchor ;
    alpha = _alpha ;
}