#pragma once

#include "ofMain.h"

class FontList
{
	public :
		FontList() { }

		void setup()
		{
            WHITNEY_BLACK = "WHITNEY_BLACK" ;
            WHITNEY_BOLD = "WHITNEY_BOLD" ;
            WHITNEY_BOOK = "WHITNEY_BOOK";
            WHITNEY_MEDIUM = "WHITNEY_MEDIUM";
            WHITNEY_SEMIBOLD = "WHITNEY_SEMIBOLD";
            WHITNEY_LIGHT = "WHITNEY_LIGHT" ;
		}

		static FontList* Instance()
        {
            static FontList inst ;
			inst.setup() ;
            return &inst ;
        }

		string WHITNEY_BLACK ;
		string WHITNEY_BOLD ;
		string WHITNEY_BOOK ;
		string WHITNEY_MEDIUM ;
		string WHITNEY_SEMIBOLD ;
		string WHITNEY_LIGHT ;
};
