#pragma once

#include "ofMain.h"
#include "FontObject.h"
#include "FontList.h"

class FontManager 
{
	public : 
		FontManager ( ) { } ; 

		static FontManager* Instance()
        {
            static FontManager inst ; 
            return &inst ; 
        }

		vector<FontObject*> fontObjects ; 

		int addFont( string path , string name ,  int size ) 
		{
			FontObject * f = new FontObject() ; 
			f->setup( path , name, size , fontObjects.size() ) ; 
			fontObjects.push_back( f ) ;
			return f->index ; 
		}

		int addFontByName( string name , int size ) 
		{
			for ( int i = 0 ; i < fontObjects.size() ; i++ ) 
			{
				if ( name == fontObjects[i]->name ) 
				{
					FontObject * f = new FontObject() ; 
					f->setup( fontObjects[i]->path , name, size, fontObjects.size() ) ; 
					fontObjects.push_back( f ) ;
					return f->index ; 
				}
			}
		}

		FontObject getFontByName( string name )
		{
			for ( int i = 0 ; i < fontObjects.size() ; i++ ) 
			{
				if ( name == fontObjects[i]->name ) 
				{
					return (*fontObjects[i]) ; 
				}
			}
		}

		FontObject getFontByIndex ( int index )
		{
			for ( int i = 0 ; i < fontObjects.size() ; i++ ) 
			{
				if ( index == fontObjects[i]->index ) 
				{
					return (*fontObjects[i]) ; 
				}
			}
		}
};