#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"

class ofxMatrixTransformTextureRef : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
	public :
        bool bInheritAlpha ;
		ofxMatrixTransformTextureRef ( );

        ~ofxMatrixTransformTextureRef ( );

		void setup ( ) ; 
		void draw ( );
		//void loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f );
        void setImageProperties( string path, float _x , float _y , ofPoint anchor , float _alpha = 0.0f  ) ;
		void setImageProperties( ofImage * img , float _x , float _y , ofPoint anchor , float _alpha = 0.0f  ) ;
		//void setImagePath ( string path ) ;
		void clear( ) ; 

		int resizeWidth , resizeHeight ; 
		
		ofPoint _anchor ;
		ofImage * texture ;
		string imageFilePath ; 
		
	
    
};
