#pragma once

#include "ofMain.h"

class ofxCalendar 
{
public : 
	ofxCalendar( ) { } 
	~ofxCalendar( ) { } 

	int month, day , year ; 

	void set( int _month, int _day , int _year ) 
	{
		month = _month ; 
		day = _day ; 
		year = _year ; 
	}

	void setToCurrent( ) 
	{
		month = ofGetMonth() ; 
		day = ofGetDay() ; 
		year = ofGetYear() ; 
	}


};