#pragma once

#include "ofMain.h"

class Quad
{
public : 
	Quad( ) { } 
	~Quad( ) { } 

	vector< ofPoint > points ;

	bool isValid() 
	{
		if ( points.size() == 4 ) return true ; 
		else return false ;
	}

	bool addPoint ( float x , float y ) 
	{
		return addPoint ( ofPoint ( x , y ) ) ; 
	}

	bool addPoint( ofPoint p ) 
	{
		cout << " add point called ! " << endl ; 
		//if ( points.size() < 4 ) 
		//{
			points.push_back( p ) ;
			return true ; 
		//}
	//	else
			return false ; 
	}

	void setPoints( vector< ofPoint > pts ) 
	{
		points = pts ; 
	} 

	string toString() 
	{
		stringstream ss ; 
		for ( int i = 0 ; i < points.size() ; i++ ) 
		{
			ss << "[" << i << "] " << points[i] << endl ;  
		}
		return ss.str() ; 
	}

};