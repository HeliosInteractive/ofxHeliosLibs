#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"

class ofxMatrixTransformImageRef : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
	public :
        bool bInheritAlpha ;
		ofxMatrixTransformImageRef ( );

        ~ofxMatrixTransformImageRef ( );

		void setup ( ) ; 
		void draw ( );
		void loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f );
        void setImageProperties( ofImage * img , float _x , float _y , ofPoint anchor , float _alpha = 0.0f  ) ;
		void setImagePath ( string path ) ;
		void clear( ) ; 


		bool bLoaded ; 
		int resizeWidth , resizeHeight ; 
		
		 ofPoint _anchor ;
		ofImage * image ;
		string imageFilePath ; 
		
	
    
};
