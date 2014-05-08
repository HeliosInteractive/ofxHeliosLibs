#pragma once
#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxImageSequence.h"
#include "Tweenzor.h"

class PositionedSequence : public ofxAlphaStackItem
{
	public : 
		PositionedSequence( ) 
		{

		}

		~PositionedSequence( ) 
		{

		}

		
		ofxImageSequence sequence ; 
		float playhead ; 
		float x  , y ; 
		float anchorX , anchorY ;
		float lengthInSeconds ; 
		string sequencePath ; 

		void setup ( string _sequencePath , float _anchorX , float _anchorY , float _x , float _y , float _lengthInSeconds ) 
		{
			sequencePath = _sequencePath ; 
			
			alpha = 0.0f; 
			anchorX = _anchorX ; 
			anchorY = _anchorY ; 
			x = _x ;
			y = _y ; 
			lengthInSeconds = _lengthInSeconds ; 
			playhead = 0.0f ; 
			sequence.loadSequence( sequencePath ) ; 
			ofxAlphaStackItem::ofxAlphaStackItem() ; 
		}

		void play ( ) 
		{
			playhead = 0.0f ; 
			alpha= 0.0f ; 
			Tweenzor::add( &alpha , 0.0f , 1.0f , 0.0f , 0.1f , EASE_OUT_QUAD ) ; 
			Tweenzor::add( &playhead , 0.0f , 1.0f , 0.0f, lengthInSeconds ) ; 
			Tweenzor::addCompleteListener( Tweenzor::getTween( &playhead ) , this , &PositionedSequence::playbackComplete ) ; 
		}

		void playbackComplete ( float * args ) 
		{
			Tweenzor::add( &alpha , alpha , 0.0f , 0.0f , 0.1f , EASE_OUT_QUAD ) ; 
		}

		void draw ( ) 
		{
			if ( alpha == 0 ) return ; 
			//cout << sequencePath << " alpha is " << getOFAlpha() << endl ; 
			ofPushMatrix() ; 
				ofSetColor( 255 , getOFAlpha() ) ;
				sequence.getFrameAtPercent( playhead )->draw(x - anchorX * sequence.getWidth() , y - anchorY * sequence.getHeight() ) ;  ; 
			ofPopMatrix() ; 
		}

		
};