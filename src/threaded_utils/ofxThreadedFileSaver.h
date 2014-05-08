#pragma once

#include "ofMain.h"
#include "ofThread.h"
#include "LoadingEvents.h"
#include "ofxSimpleTimer.h"

class ofxThreadedFileSaver : public ofThread 
{
	public :
		ofxThreadedFileSaver( ) { } 
		~ofxThreadedFileSaver( ) { } 

		void threadedFunction( ) ;
		void saveFileToPath ( string path , ofBuffer fileBufferData , bool bIsBinary , string _originalUrl ) ;
		void update() ; 
		void retry() ; 

		ofxSimpleTimer timeoutTimer ; 
	private :
		int attempts ; 
		bool bIsBinary ;  
		ofBuffer bufferData ; 
		string saveToPath ; 
		ofFile writeToFile ; 
		string originalUrl ; 


};