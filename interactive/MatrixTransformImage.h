#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"


class MatrixTransformImage : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
	public :
        bool bInheritAlpha ;
		MatrixTransformImage ( )
		{
			scale = 1.0f ;
			rotation = 0.0f ;
			bInheritAlpha = false ;
			//alpha = 0 ;
		}

		void loadImage( string path , float _x , float _y , ofPoint anchor , float _alpha = 0.0f )
		{
		    image.loadImage( path ) ;
		    x = _x ;
		    y = _y ;
		    image.setAnchorPercent( anchor.x , anchor.y ) ;
            alpha = _alpha ; 
		}

		ofImage image ;

		void draw ( )
		{
			matrixPush( ) ;
              //  if ( !bInheritAlpha )
                ofSetColor ( 255 , 255 , 255 , getOFAlpha() ) ;
				image.draw ( 0 , 0 ) ;
			matrixPop( ) ;
		}
    
        void resizePorportionally ( float _width , float _height )
        {
            if ( image.bAllocated() )
            {
                
            }
            
        }
};
