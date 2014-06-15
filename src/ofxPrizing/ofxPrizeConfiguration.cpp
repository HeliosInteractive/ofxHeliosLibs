#include "ofxPrizeConfiguration.h"

void ofxPrizeConfiguration::loadFromCSV( string fileName ) 
{
	//load CSV into buffer
	csv = ofBufferFromFile( fileName ) ;
}

//Only supports one prize per day currently
ofxPrizeData ofxPrizeConfiguration::getTodaysPrizeData( bool bUseDefault ) 
{
	if ( csv.size() == 0 ) 
	{
		ofLogError( "ofxPrizeConfiguration::getTodaysPrizeData() " ) << " BUFFER IS NOT LOADED error." ; 
		return ofxPrizeData() ; 
	}

	//Default prize is mostly for debugging...
	ofxPrizeData defaultPrize ; 

	//skip the first row as it's labels 
	csv.getNextLine() ; 
	int row = 1 ; 
	//Loop until we reach the end of the CSV
	while( !csv.isLastLine() ) 
	{
		string line = csv.getNextLine() ; 
		vector< string > columns = ofSplitString( line , "," , false ) ; 

		ofxCalendar date ; 
		date.set( ofToInt( columns[0] ) , ofToInt( columns[1] ) , ofToInt( columns[2] ) , 0 , 0  ) ; 
		ofLogVerbose() << "date on ROW " << row << " is : " << date.toString() ; 

		ofxCalendar startTime = date ; 
		startTime.hour = ofToInt( columns[3] ) ; 
		startTime.minute = ofToInt( columns[4] ) ; 

		ofxCalendar endTime = startTime ; 
		endTime.hour = ofToInt( columns[5] ) ; 
		endTime.minute = ofToInt( columns[6] ) ; 
		/*
		for ( int c = 0 ; c < columns.size() ; c++ ) 
		{
			cout << "[" << c << "]" << columns[c] << " " ; 
		}
		
		ofLogVerbose() << "startTime : " << startTime.getClockString() ; 
		ofLogVerbose() << "endTime : " << endTime.getClockString() ; 
		*/

		//Initialize the default prize incase there is not a match for today.
		if ( row == 1 ) 
		{
			ofxCalendar today ; 
			today.setToCurrent() ; 
			//Override default prize info with today's date so all the labels match
			startTime.year = endTime.year = today.year ; 
			startTime.month = endTime.month = today.month ; 
			startTime.day = endTime.day = today.day ; 
		
			defaultPrize.initialize( columns[7] , ofToInt( columns[8] ) , startTime, endTime ) ;
		}
		
		if ( date.isToday() ) 
		{
			ofLogNotice( ) << " prize found for TODAY ! loading prize info for '" << columns[8] << "'" ; 
			ofxPrizeData prize ; 
			prize.initialize( columns[7] , ofToInt( columns[8] ) , startTime, endTime ) ;
			return prize ; 
		}
		row++ ; 
	}	

	//If it's goten this far, that means to match for today was found return default 
	ofLogNotice( "no prize found for today, returning default prize" ) ; 
	if ( bUseDefault ) 
		return defaultPrize ;
	else 
		return ofxPrizeData() ; 
}