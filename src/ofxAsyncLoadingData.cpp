#include "ofxAsyncLoadingData.h"

void ofxAsyncLoadingData::setup ( string _url , int _uniqueId , bool _bBinaryFile )
{
	bLoaded = false ; 
	numLoadAttempts = 0 ; 
    url = _url ; 
	uniqueId = _uniqueId ; 
	bBinaryFile = _bBinaryFile ; 
	startLoadTime = ofGetSystemTime() ; 
}

