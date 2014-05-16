#include "IdleInputManager.h"

void IdleInputManager::setup( float delayInMillis ) 
{
	idleTimer.setup( delayInMillis , "IDLE MANAGER INPUT TIMER" ) ;
	ofAddListener( idleTimer.TIMER_COMPLETE , this , &IdleInputManager::idleTimerComplete ) ; 
	bEnabled = false ; 
}

void IdleInputManager::update( ofEventArgs &data ) 
{
	idleTimer.update( ) ; 
}

void IdleInputManager::idleTimerComplete ( int &args ) 
{
	if ( bEnabled == true ) 
	{
		int args = 15 ; 
		ofNotifyEvent( IDLE_TIMEOUT , args ) ; 
	}
}

void IdleInputManager::input ( ) 
{
	if ( bEnabled == true ) 
	{
		idleTimer.start( false , true ) ; 
	}
}

void IdleInputManager::enable( ) 
{
	bEnabled = true ; 
}

void IdleInputManager::disable( ) 
{
	bEnabled = false ; 
	idleTimer.stop( ) ; 
}