#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"

class ofxMatrixTransformImage : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
	public :
        bool bInheritAlpha ;
		ofxMatrixTransformImage ( )
		{
			scale = 1.0f ;
			rotation = 0.0f ;
			bInheritAlpha = false ;
			//alpha = 0 ;
		}
    
        ~ofxMatrixTransformImage ( )
        {
            image.clear() ; 
        }

		void loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f )
		{
		    image.loadImage( path ) ;
		    x = _x ;
		    y = _y ;
            _anchor = anchor ;
//		    image.setAnchorPercent( anchor.x , anchor.y ) ;
            alpha = _alpha ;
            //cout << "loadImage : " << path << endl ;
		}
    
        ofPoint _anchor ;

        void setImageProeprties( float _x , float _y , ofPoint anchor , float _alpha = 0.0f )
        {
            x = _x ;
            y = _y ;
            _anchor = anchor ;
            //image.setAnchorPercent( anchor.x , anchor.y ) ;
            alpha = _alpha ;
        }
    
		ofImage image ;

		void draw ( )
		{
			matrixPush( ) ;
              //  if ( !bInheritAlpha )
                ofSetColor ( 255 , 255 , 255 , getOFAlpha() ) ;
                image.draw ( image.width * -_anchor.x , image.height * -_anchor.y ) ;
               // image.draw ( image.width * _anchor.x , image.height * _anchor.y ) ;
			matrixPop( ) ;
		}
    
        void resizePorportionally ( float _width , float _height )
        {
            if ( image.bAllocated() )
            {
                
            }
            
        }
    
};
