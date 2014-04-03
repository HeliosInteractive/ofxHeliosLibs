#pragma once

#include "ofMain.h"

class ofxHeliosUtils
{
public : 

	ofxHeliosUtils( ) { } 
	~ofxHeliosUtils( ){ } 

	static float wrapDegrees( float angle ) 
	{
		float theta = ofWrapDegrees( angle ) ; 
		if ( theta < 0 ) 
		{
			theta = ofMap( theta , -180.0f , 0.0f , 180.0f , 360.0f , true ) ; 
		}

		return theta ; 
	}
};