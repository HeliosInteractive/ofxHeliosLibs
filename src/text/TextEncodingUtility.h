//
//  TextEncodingUtility.h
//  BE_Aerospace_Platform
//
//  Created by Ben McChesney on 4/2/13.
//
//

#ifndef BE_Aerospace_Platform_TextEncodingUtility_h
#define BE_Aerospace_Platform_TextEncodingUtility_h

#include "ofMain.h"
#include "TextEncodingPairing.h"

class TextEncodingUtility
{
    public  :
        TextEncodingUtility( ) { }
        ~TextEncodingUtility( ) { }
    
        static TextEncodingUtility* Instance()
        {
            static TextEncodingUtility inst ;
            return &inst ;
        }
    
        vector< TextEncodingPairing > toReplace ;

        void addEncodingPair ( string rawText , string renderedText )
        {
            toReplace.push_back( TextEncodingPairing( rawText , renderedText ) ) ; 
        }

        string checkEncoding ( string input )
        {
            string output = input ;
            
            for ( int i = 0 ; i < toReplace.size() ; i++ )
            {
                //cout << "searching for " << toReplace[i].renderText << endl ;
                int searchIndex = output.find( toReplace[i].rawText ) ;
                int numReplaced = 0 ; 
                while ( searchIndex != -1 && searchIndex < output.size()  )
                {
                    output.replace( searchIndex , toReplace[i].rawText.size() ,  toReplace[i].renderText.c_str()  ) ; //toReplace[i].rawText.size() ) ;
                    numReplaced++ ; 
                    //cout << "after size() " << output.size( ) << endl ;
                    //cout << "output : " << output << endl ;
                    int newSearchIndex = output.find( toReplace[i].rawText , searchIndex + 1 ) ;
                    if( newSearchIndex > searchIndex )
                    {
                        //cout << " new searchIndex : " << newSearchIndex << " old : " << searchIndex << endl ;
                        searchIndex = newSearchIndex ;
                    }
                    else
                    {
                        searchIndex = -1 ;
                    }
                }
                
               // if ( numReplaced > 0 )
               //     cout << " replaced #" << numReplaced << " " << toReplace[i].renderText << endl ;
            }
            
            //if ( input.compare( output ) != 0 )
           // {
           //     cout << "input : " << input << endl << "output : " << output << endl ;
           // }
            return output ;
        }

/*
 cout << "does : " << checkMe << " have a weird symbol ? " << endl ;
 int ampIndex = checkMe.find( "&amp;" ) ;
 if ( ampIndex >= 0 )
 {
 cout << " FOUND an ampersand ! " << endl ;
 // checkMe.insert( ampIndex , "™" , 4 ) ;
 }
 
 int tmIndex = checkMe.find( "(tm)" ) ;
 if ( tmIndex >= 0 )
 {
 cout << " FOUND an TM ! " << endl ;
 //checkMe.insert( tmIndex , "™" , 2 ) ;
 }
 
 int aposIndex = checkMe.find( "ât" ) ;
 if ( aposIndex >= 0 )
 {
 cout << " FOUND an apostrophe ! " << endl ;
 checkMe.insert( aposIndex , "'" , 4 ) ;
 }
 
 return checkMe ;
*/


};


#endif
