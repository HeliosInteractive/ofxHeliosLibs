#pragma once

#include "ofMain.h"

class ofxMatrixTransformObject : public ofPoint
{
	public :
        ofxMatrixTransformObject ( )  { }
        ~ofxMatrixTransformObject ( )  { }
    
        void setup ( ) ; 
        void pushMatrix( ) ;
        void popMatrix ( ) ;

        virtual void setHitBounds ( ofRectangle _rect ) ;
        virtual ofRectangle getHitBounds ( ) ;
        virtual bool hitTest( ofVec2f p , ofRectangle _bounds ) ;
		virtual bool hitTest( ofVec2f p ) ; //, ofRectangle _bounds ) ;
    
        float rotation ;	// z rotation
        float scale ;		// uniform scale
        //float x , y ;		// caRtesian points
    
        ofRectangle hitBounds ;
    
};
