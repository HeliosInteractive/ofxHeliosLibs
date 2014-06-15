#pragma once

/* 
	
	Created by Ben McChesney of Helios Interactive 6/14/2014

*/

#include "ofxCalendar.h"
#include "ofxPrizeData.h"

class ofxPrizeConfiguration
{
public : 
	ofxPrizeConfiguration( ) { } 
	~ofxPrizeConfiguration( ) { } 

	void loadFromCSV( string fileName ) ; 
	ofxPrizeData getTodaysPrizeData( bool bUseDefault = true ) ; 
	ofBuffer csv ; 
	
};