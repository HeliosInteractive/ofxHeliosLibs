#pragma once

#include "RoundedGradientPanel.h"
#include "GradientQuad.h"
#include "SlidingDoorRect.h"
#include "ofxSimpleMask.h"
#include "ofxAlphaStackItem.h"

class RoundedGradientPanel : public ofxAlphaStackItem
{
public : 
	RoundedGradientPanel() { } 
	~RoundedGradientPanel() {}

		void setup( ofRectangle area , float padding , float cornerRadius )  ;
		void update() ;
		void draw() ;

	
		GradientQuad quad ; 
		ofxSimpleMask mask ; 
		ofFbo maskFbo ; 

		void transitionIn( ) ;  
		void transitionOut() ; 
		ofRectangle rect ; 
		float cornerRadius ; 
		float padding ;
};