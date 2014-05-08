#pragma once

#include "ofMain.h"

class ofxAsyncLoadingData
{
    public:
		ofxAsyncLoadingData()
		{ 
			
		} 
		~ofxAsyncLoadingData() { } 

        void setup ( string _url , int _uniqueId, bool _bBinaryFile ) ; 

		//Has this successfully Loaded ?
        bool bLoading ; 

		//Has there been an attempt to load this ?
		int numLoadAttempts ; 

		//Url we are after
        string url ;

		//Easier to compare integers than strings
        int uniqueId ;

		//For when we need to save it to disk
		bool bBinaryFile ;

		unsigned long startLoadTime ; 
		int totalLoadTime ; 
		

    protected:
    private:
};

