#pragma once

#include "ofMain.h"

class ofxHeliosUtils
{
public : 

	ofxHeliosUtils( ) { } 
	~ofxHeliosUtils( ){ } 

	static float wrapDegreesTo360( float angle ) 
	{
		float theta = ofWrapDegrees( angle ) ; 
		if ( theta < 0 ) 
		{
			theta = ofMap( theta , -180.0f , 0.0f , 180.0f , 360.0f , true ) ; 
		}

		return theta ; 
	}

	static void contrainImageToDimensions( ofImage * image , float maxWidth , float maxHeight ) 
	{
		if ( image == NULL || image->bAllocated() == false ) return ;

		float aspectRatio = image->getWidth() / image->getHeight() ; 
		cout << "MAX image dims : " << maxWidth << " x " << maxHeight << endl ; 
		cout << " image dims : " << image->getWidth() << " x " << image->getHeight() << endl ;
		if ( image->getWidth() > maxWidth ) 
		{
			float scale = maxWidth / image->getWidth()  ; 
			image->resize( maxWidth , image->getHeight() * scale ) ; 
			cout << " TOO WIDE -  dims : " << image->getWidth() << " x " << image->getHeight() << endl ;
		}
		if ( image->getHeight() > maxHeight ) 
		{
			float scale = maxHeight / image->getHeight()   ; 
			image->resize( image->getWidth() * scale , maxHeight ) ; 
			cout << " TOO TALL - " << image->getWidth() << " x " << image->getHeight() << endl ;
		}
	}
};