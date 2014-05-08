#pragma once

#include "ofMain.h"

class PageEventData
{
public : 
	PageEventData( int _id , vector< string > _data ) 
	{
		id = _id ; 
		data = _data ; 
	}

	int id ; 
	vector< string > data ; 
};