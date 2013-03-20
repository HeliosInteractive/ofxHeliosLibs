#pragma once

#include "ofMain.h"
#include "AlphaStackItem.h"
#include "MatrixTransformObject.h"


class MatrixTransformImage : public AlphaStackItem , public MatrixTransformObject
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

		void loadImage( string path , float _x , float _y , ofPoint anchor )
		{
		    image.loadImage( path ) ;
		    x = _x ;
		    y = _y ;
		    image.setAnchorPercent( anchor.x , anchor.y ) ;
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
};
