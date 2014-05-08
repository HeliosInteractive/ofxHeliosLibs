#pragma once

#include "ofxOsc.h"
#include "GestureCursor.h"
#include "GestureEvents.h"

class GestureHub
{
public : 
	GestureHub( ) { } 
	~GestureHub( ) { } 


	static enum INPUT_MODE
	{
		OSC = 0 
	};

	void setup ( INPUT_MODE mode ) ; 
	void update( ) ; 
	void draw( ) ; 

	int createCursor( ) ;

private : 
	vector < GestureCursor * > cursors ; 
	ofxOscReceiver oscReciever ; 

};