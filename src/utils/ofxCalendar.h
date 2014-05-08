#pragma once

#include "ofMain.h"

class ofxCalendar 
{
public : 
	ofxCalendar( ) { } 
	~ofxCalendar( ) { } 

	//Calendar
	int month, day , year ;

	//Time of Day
	int minute , hour ;  

	void set( int _month, int _day , int _year , int _hour = 12 , int _minute = 0 ) 
	{
		month = _month ; 
		day = _day ; 
		year = _year ; 
		hour = _hour ;
		minute = _minute ; 
	}

	void setToCurrent( ) 
	{
		month = ofGetMonth() ; 
		day = ofGetDay() ; 
		year = ofGetYear() ; 
		hour = ofGetHours() ; 
		minute = ofGetMinutes() ; 
	}

	string log ( ) 
	{
		stringstream ss; 
		
		ss << " M/D/Y " << month << " / " << day << " / " << year << " @ " << hour << ":" << minute << endl ; 
		return ss.str() ; 
	}


};