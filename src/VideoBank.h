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

	void setup ( float x , float y , float timerDelay , bool bLoop ) ; 
	void loadFolder( string folderPath , float _volume = 1.0f ) ; 
	void addVideo( string videoPath , float _volume = 1.0f ) ;
	void update( ) ; 
	void draw( ) ; 
	void playRandomVideo( ) ; 
	void playVideoAt( int index ) ; 
	void stop ( ) ; 
	void reset( ) ; 

	bool bLoop ; 

	float volume ; 
	float x , y ; 
	
	ofxSimpleTimer videoDelayTimer ; 
	void videoDelayTimerComplete( int & args ) ; 

	vector< ofVideoPlayer * > videos ; 

	int currentVideo ; 
	int lastRandomIndex ; 

};