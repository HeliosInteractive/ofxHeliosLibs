#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"

class ofxMatrixTransformImage : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
	public :
        bool bInheritAlpha ;
		ofxMatrixTransformImage ( )

        ~ofxMatrixTransformImage ( )
		void draw ( )
		void loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f )
        void setImageProperties( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f  ) 
		void setImagePath ( string path ) 	
		bool isNewlyLoaded( ) 


		bool bLoaded ; 
		int resizeWidth , resizeHeight ; 
		
		 ofPoint _anchor ;
		ofImage image ;
		string imageFilePath ; 
		
	
    
};
