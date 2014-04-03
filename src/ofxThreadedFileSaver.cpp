#include "ofxThreadedFileSaver.h"


void ofxThreadedFileSaver::threadedFunction( ) 
{
	stringstream ss ; 
	while ( isThreadRunning() != 0 ) 
	{
		unsigned int long start = ofGetSystemTime() ; 
		if ( lock() ) 
		{
			ofFile writeToFile = ofFile( saveToPath ) ; //"" , ofFile::WriteOnly ) ;
			writeToFile.create() ; 
			writeToFile.changeMode( ofFile::WriteOnly , bIsBinary ) ; 
		
			bool bResult = writeToFile.writeFromBuffer( bufferData ) ;
			writeToFile.close() ;
		 
			ss << " bResult : " << bResult << " after saving to : " << saveToPath << endl ; 
			unlock() ; 
			stopThread( ) ; 

			ofNotifyEvent( LoadingEvents::Instance()->FILE_LOADED , originalUrl ) ; 
		}


		ss << "ending thread. Saving to disk took: " << ( ofGetSystemTime() - start )  << endl ; 
		ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
	}
}

void ofxThreadedFileSaver::update()
{
	timeoutTimer.update(); 
}


void ofxThreadedFileSaver::saveFileToPath ( string path , ofBuffer fileBufferData , bool _bIsBinary, string _originalUrl ) 
{
	attempts = 0 ; 
	bufferData = fileBufferData ; 
	saveToPath = path ; 
	bIsBinary = _bIsBinary ; 
	originalUrl = _originalUrl ; 
	timeoutTimer.setup( 9000 , "noName" , 18 , false ) ; 
	// void setup ( unsigned long delayInMillis , string _name = "noName" , int eventId = 18 , bool bUseTweenzor = true ) ;
	timeoutTimer.start( false , true ) ; 
	startThread( true , false ) ; 
}

void ofxThreadedFileSaver::retry() 
{
	attempts++ ; 
	if ( attempts > 4 ) 
	{
		stringstream ss ; 
		ss << saveToPath << " has failed to save to the computer 5 times..." << endl ; 
		ofLogVerbose( ss.str() ) ; 
	}

	waitForThread( true ) ; 
	timeoutTimer.start( false , true ) ; 
	startThread( true , false ) ; 
}


