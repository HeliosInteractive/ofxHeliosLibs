#pragma once

#include "ofMain.h"
#include "ofxAsyncLoadingData.h"
#include "ofxThreadedFileSaver.h"
#include "LoadingEvents.h"
#include "ofxThreadedImageLoader.h"
#include "ofxSimpleTimer.h"

class ofxLoadingManager
{
	public : 
		ofxLoadingManager( ) {}

		

		//Core OF Functions
		void setup ( string localSavePath ) ; 
		void setupLoadingDirectory ( string loadDirPath ) ; 
		void update() ; 
		void draw() ; 

		void loadURL( string _url ) ;
		void urlResponse( ofHttpResponse & response ) ; 
		bool checkFileExtensionForBinary( string filePath ) ; 

		string remoteUrlToLocal( string url ) ; 
		string loadDirectoryPath ; 
	
		int generateUniqueId() ; 
		int currentId ;  

		vector<string> textFileExtensions ; 
		vector<string> binaryFileExtensions ; 

		//Threaded things can't be in a vector....
		ofxThreadedFileSaver * * fileSavers ;
		int numFileSavers ; 

		ofxThreadedImageLoader * threadedLoader ; 
		bool addToThreadedImageQueue( ofImage &  image , string path , bool bAddToQueue = true ) ; 

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
		vector<ofxAsyncLoadingData> cacheDataQueue ; 
		vector<ofxAsyncLoadingData> writingData ; 
		vector<ofxAsyncLoadingData> completedData ; 

		int getAsyncDataIndexFromUrl( string url ) ; 

		ofDirectory localDirectory ; 
		string localDirectoryPath ; 

		void removeAsyncDataFromQueue( ofxAsyncLoadingData data ) ; 
		void loadCompleteHandler( string &args ) ; 

		void timerCompleteHandler( int &args ) ; 

		float totalLoadTime; 

		ofxSimpleTimer checkFileLoopTimer ; 
		void checkFileLoopComplete ( int &args ) ; 

		vector< ofImage * > threadedQueueImageRefs ; 
		void stop ( ) ; 
		
};