#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"

class VideoBank
{
public : 
	VideoBank( ) 
	{
		lastRandomIndex = -1 ;
		currentVideo = -1 ; 
	}
	~VideoBank ( ) { } 

	void setup ( float x , float y , float timerDelay ) ; 
	void loadFolder( string folderPath , float _volume = 1.0f ) ; 
	void addVideo( string videoPath , float _volume = 1.0f ) ;
	void update( ) ; 
	void draw( ) ; 
	void playRandomVideo( ) ; 

	float volume ; 
	float x , y ; 
	
	ofxSimpleTimer videoDelayTimer ; 
	void videoDelayTimerComplete( int & args ) ; 

	vector< ofVideoPlayer * > videos ; 

	int currentVideo ; 
	int lastRandomIndex ; 

};