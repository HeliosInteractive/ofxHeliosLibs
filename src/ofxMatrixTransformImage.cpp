#include "ofxMatrixTransformImage.h"


ofxMatrixTransformImage::ofxMatrixTransformImage ( )
{
	scale = 1.0f ;
	rotation = 0.0f ;
	bInheritAlpha = false ;
	//alpha = 0 ;
}
    
ofxMatrixTransformImage::~ofxMatrixTransformImage ( )
{
    image.clear() ; 
}

void ofxMatrixTransformImage::draw ( )
{
	matrixPush( ) ;
        ofSetColor ( 255 , 255 , 255 , getOFAlpha() ) ;
        image.draw ( image.width * -_anchor.x , image.height * -_anchor.y ) ;
	matrixPop( ) ;
}

void ofxMatrixTransformImage::loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha )
{
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


		