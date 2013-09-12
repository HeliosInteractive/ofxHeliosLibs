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
		}


		ss << "ending thread. Saving to disk took: " << ( ofGetSystemTime() - start )  << endl ; 
		ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
	}
}


void ofxThreadedFileSaver::saveFileToPath ( string path , ofBuffer fileBufferData , bool _bIsBinary ) 
{
	bufferData = fileBufferData ; 
	saveToPath = path ; 
	bIsBinary = _bIsBinary ; 
	startThread( true , false ) ; 
}


