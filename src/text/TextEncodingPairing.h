//
//  TextEncodingPairing.h
//  BE_Aerospace_Platform
//
//  Created by Ben McChesney on 4/2/13.
//
//

#ifndef BE_Aerospace_Platform_TextEncodingPairing_h
#define BE_Aerospace_Platform_TextEncodingPairing_h

#include "ofMain.h"

class TextEncodingPairing
{
    public :
        TextEncodingPairing( string _rawText , string _renderText )
        {
            rawText = _rawText ;
            renderText = _renderText ; 
        }
        
        
        string rawText ;
        string renderText ;
    
};
#endif
