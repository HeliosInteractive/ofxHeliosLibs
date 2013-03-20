#pragma once

#include "ofMain.h"

class ofxMatrixTransformObject
{
	public :
        ofxMatrixTransformObject ( )  { }
        ~ofxMatrixTransformObject ( )  { }
    
        void setup ( ) ; 
        void matrixPush( ) ;
        void matrixPop ( ) ;

        virtual void setHitBounds ( ofRectangle _rect ) ;
        virtual ofRectangle getHitBounds ( ) ;
        virtual bool hitTest( ofVec2f p , ofRectangle _bounds ) ;
    
        float rotation ;	// z rotation
        float scale ;		// uniform scale
        float x , y ;		// catersian points
    
        ofRectangle hitBounds ;
    
};
