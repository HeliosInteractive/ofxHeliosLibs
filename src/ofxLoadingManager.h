#pragma once

#include "ofMain.h"
#include "ofxAsyncLoadingData.h"
#include "ofxThreadedFileSaver.h"

#ifdef OSC_LOAD_NOTIFICATIONS
#include "ofxOsc.h"
#endif

class ofxLoadingManager
{
	public : 
		ofxLoadingManager( ) {}

		static ofxLoadingManager* Instance()
        {
            static ofxLoadingManager inst ; 
            return &inst ; 
        }

		//Core OF Functions
		void setup ( string localSavePath ) ; 
		void update() ; 
		void draw() ; 

		void loadURL( string _url ) ;
		void urlResponse( ofHttpResponse & response ) ; 
		bool checkFileExtensionForBinary( string filePath ) ; 
		string remoteUrlToLocal( string url ) ; 

	
		int generateUniqueId() ; 
		int currentId ;  

		vector<string> textFileExtensions ; 
		vector<string> binaryFileExtensions ; 

		//Threaded things can't be in a vector....
		ofxThreadedFileSaver * * fileSavers ;
		int numFileSavers ; 

		//So this is my super hacky method of having 10 threaded object savers at any given time 
		ofxThreadedFileSaver saver0 ;
		ofxThreadedFileSaver saver1 ; 
		ofxThreadedFileSaver saver2 ; 
		ofxThreadedFileSaver saver3 ;
		ofxThreadedFileSaver saver4 ;
		ofxThreadedFileSaver saver5 ;
		ofxThreadedFileSaver saver6 ;
		ofxThreadedFileSaver saver7 ;
		ofxThreadedFileSaver saver8 ;
		ofxThreadedFileSaver saver9 ;
		
		int getNextEmptyFileSaverIndex() ; 
		vector<ofxAsyncLoadingData> asyncLoadData ; 

		int getAsyncDataIndexFromUrl( string url ) ; 

		ofDirectory localDirectory ; 
		string localDirectoryPath ; 

		
};