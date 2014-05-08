#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSimpleTimer.h"
#include "Tweenzor.h"
#include "GestureEvents.h"

class GestureCursor 
{
public : 
	GestureCursor( ) ; 
	~GestureCursor ( ) ;

	void setup ( int id ) ; 
	void update ( ) ; 
	void drawScreen ( ) ; 
	void draw3D( ) ; 

	int id ; 
	void parseOscMessage( ofxOscMessage * m ) ; 

	ofPoint normalizedScreen ; 
	ofPoint screenPosition ;

	ofPoint normalizedWorld ; 
	ofPoint worldPosition ; 

	float worldScale ;
	float smoothing ; 

private : 
	enum CURSOR_STATUS 
	{
		LOST = 0 ,
		OPEN = 1 , 
		CLOSED = 2 , 
		LASSO = 3 ,
		CALIBRATED = 4
	} ;

	CURSOR_STATUS status ;

	ofxSimpleTimer dragLostTimer ; 
	void dragLostTimerComplete ( int & args ) ; 
};