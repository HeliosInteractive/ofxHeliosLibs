#include "ofxMatrixTransformImage.h"


ofxMatrixTransformImage::ofxMatrixTransformImage ( )
{
	scale = 1.0f ;
	rotation = 0.0f ;
	bInheritAlpha = false ;
	setup( ) ; 
	//alpha = 0 ;
}
    
ofxMatrixTransformImage::~ofxMatrixTransformImage ( )
{
    image.clear() ; 
}

void ofxMatrixTransformImage::setup ( ) 
{
	ofxMatrixTransformObject::setup() ;
	ofxAlphaStackItem::setup ( ) ; 

	scale = 1.0f ;
	rotation = 0.0f ;
	bInheritAlpha = false ;

}

void ofxMatrixTransformImage::draw ( )
{
	pushMatrix( ) ;
        ofSetColor ( 255 , 255 , 255 , getOFAlpha() ) ;
		if ( image.bAllocated() ) 
		    image.draw ( image.width * -_anchor.x , image.height * -_anchor.y ) ;
	popMatrix( ) ;
}

void ofxMatrixTransformImage::loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha )
{
	if ( path != "" ) 
		image.loadImage( path ) ;
	x = _x ;
	y = _y ;
    _anchor = anchor ;
    alpha = _alpha ;
	imageFilePath = path ; 
}

void ofxMatrixTransformImage::setImageProperties( string path , float _x , float _y , ofPoint anchor , float _alpha  )
{
    x = _x ;
    y = _y ;
    _anchor = anchor ;
    alpha = _alpha ;
			
	bLoaded = false ; 
	setImagePath ( path ) ; 
}

void ofxMatrixTransformImage::clear( ) 
{
	bLoaded = false ; 
	image.clear() ; 
}

void ofxMatrixTransformImage::setImagePath ( string path ) 
{
	//Could do more here
	//Check if it's the same Path ? Clear the image if it's already load, etc 
	imageFilePath = path ;
}
    
bool ofxMatrixTransformImage::isNewlyLoaded( ) 
{
	if ( bLoaded == false ) 
	{
		if ( image.getWidth() > 0 && image.getHeight() > 0 ) 
		{
			bLoaded = true ; 
			return true ; 
		}
	}

	return false ; 
}


		