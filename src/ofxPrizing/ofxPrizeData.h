#pragma once


#include "ofMain.h"

class ofxPrizeData
{
public : 
	ofxPrizeData() { reset() ; } 
	~ofxPrizeData( ) { } 

	void reset ( ) 
	{
		totalPrizes = 0 ; 
		awardedPrizes = 0 ; 
		stashedPrizes = 0 ; 
		
		prizeName = "no name" ;
	}
	
	string toString( ) 
	{
		stringstream ss ; 
		ss << "Prize '" << prizeName << "' - " << " TOTAL prizes: #" << totalPrizes << " awarded : #" << awardedPrizes << " stashed : #" << stashedPrizes << endl ; 
		return ss.str() ; 
	}

	void initialize( string _prizeName ,  int _totalPrizes , ofxCalendar _startTime , ofxCalendar _endTime ) 
	{
		reset() ; 
		prizeName = _prizeName ; 
		totalPrizes = _totalPrizes ; 
		startTime = _startTime ; 
		endTime = _endTime ;
	}


	int totalPrizes ;
	int awardedPrizes ;
    int stashedPrizes ;

	ofxCalendar startTime ; 
    ofxCalendar endTime ; 
	string prizeName ; 

};