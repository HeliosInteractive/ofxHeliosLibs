#pragma once

#include "ofMain.h"

class FontObject
{
	public : 
		FontObject() { }  

		void setup( string _path ,  string _name, int _size  , int _index ) 
		{
			path = _path ; 
			index = _index ; 
			name = _name ;
			size = _size ;
			font.loadFont( path , size , true ) ; 
		}

		ofTrueTypeFont font ; 
		int size ; 
		string path ; 
		int index ; 
		string name	;
};