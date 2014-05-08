#pragma once

#include "ofMain.h"

class ofxThreadedLoader : public ofThread
{
public : 
	ofxThreadedLoader( ) { } 
	~ofxThreadedLoader( ) { }

	void threadedFunction () 
	{
		while ( isThreadRunning() != 0 ) 
		{
			if ( lock() ) 
			{
				ofHttpResponse response = ofLoadURL( urlToLoad ) ; 
				unlock() ; 
				stopThread( ) ; 
				ofURLResponseEvent().notifyAsync( this , response ) ; 
				//ofNotifyEvent( ofURLResponseEvent().
			}
		}
	}

	void loadFile( string _urlToLoad ) 
	{
		urlToLoad = _urlToLoad ; 
		startThread( true , false ) ; 
	}

	string urlToLoad ;
};