#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"

class IdleInputManager
{
public : 
	static IdleInputManager* Instance()
    {
        static IdleInputManager inst ;
        return &inst ;
    }

	void setup( float delayInMillis ) ;
	void update( ofEventArgs &data ) ; 
	void idleTimerComplete ( int &args ) ; 
	void input ( ) ; 
	void enable( ) ; 
	void disable( ) ; 

	ofxSimpleTimer idleTimer ; 
	bool bEnabled ; 

	ofEvent< int > IDLE_TIMEOUT ; 

private : 
	
	IdleInputManager( )
	{
		//Keeps it easy so we know that this will exist and continue to update.
		ofAddListener( ofEvents().update , this , &IdleInputManager::update ) ; 
	} 

	~IdleInputManager( )
	{
		ofRemoveListener( ofEvents().update , this , &IdleInputManager::update ) ; 
	} 
};
