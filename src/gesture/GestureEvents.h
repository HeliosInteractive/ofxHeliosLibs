#pragma once
#include "ofMain.h"

class GestureEvents
{
public : 
	~GestureEvents( ) { } 

	ofEvent< GestureEventArgs > DOWN ; 
	ofEvent< GestureEventArgs > DRAG ; 
	ofEvent< GestureEventArgs > UP ; 

	static GestureEvents* Instance()
    {
        static GestureEvents inst ;
        return &inst ;
    }

private :
	GestureEvents( ) { } 


};

class GestureEventArgs
{
public : 
	GestureEventArgs( int _id ) 
	{
		id = _id ; 
	} 

	int id ; 
};