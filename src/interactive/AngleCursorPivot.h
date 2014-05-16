#pragma once

#include "ofMain.h"
#include "BaseDepthInteractive.h"
#include "MatrixTransformObject.h"

class AngleCursorPivot : public BaseDepthInteractive ,  public MatrixTransformObject
{
	public : 
		AngleCursorPivot() { }

		
		float angleToInput ; 
		ofImage angleSheen ; 

		void loadAngleSheen ( string path ) 
		{
			angleSheen.loadImage( path) ; 
			angleSheen.setAnchorPercent( 0.5f , 0.5f ) ;
			angleToInput = 0.0f ; 
		}

		void drawSheen ( ) 
		{
			if ( angleSheen.bAllocated() == false ) return ; 
			ofPushMatrix( ) ; 
				ofRotateZ( angleToInput ) ; 
				angleSheen.draw ( 0 , 0 ) ; 
			ofPopMatrix( ) ; 
		}

		void updateAngle ( float x2 , float y2 ) 
		{
			angleToInput = ofRadToDeg( atan2(  y2 - y , x2  - x ) ) ;
		}
}; 