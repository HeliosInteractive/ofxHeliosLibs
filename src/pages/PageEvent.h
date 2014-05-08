#pragma once

#include "ofMain.h"
#include "PageEventData.h"
#include "AppConstants.h"
class PageEvent
{
public : 
	PageEvent( )
	{
	
	} 

	static PageEvent* Instance()
    {
        static PageEvent inst ;
        return &inst ;
    }

	ofEvent< int > ENABLE_INPUT ; 
	ofEvent< int > DISABLE_INPUT ; 
	ofEvent< PageEventData > NAVIGATE ; 
	ofEvent< PageEventData > RESET ; 

};