#pragma once

#include "ofMain.h"
#include "ofThread.h"

class ofxThreadedFileSaver : public ofThread 
{
	public :
		ofxThreadedFileSaver( ) { } 
		~ofxThreadedFileSaver( ) { } 

		void threadedFunction( ) ;
		void saveFileToPath ( string path , ofBuffer fileBufferData , bool bIsBinary ) ;
		bool getIsRunning( ) ; 


	private :
		bool bIsBinary ;  
		ofBuffer bufferData ; 
		string saveToPath ; 
		ofFile writeToFile ; 

};