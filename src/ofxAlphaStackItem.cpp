//
//  AlphaStackItem.cpp
//  BE_Aerospace
//
//  Created by Ben McChesney on 3/19/13.
//
//

#include "ofxAlphaStackItem.h"

void ofxAlphaStackItem::setup( )
{
    alpha = 1.0f;
    calcAlpha = 1.0f ;
}


//Most basic update multiplies stackAlpha by current alpha
//Override this and pass along to children is need be
void ofxAlphaStackItem::alphaStackUpdate ( float stackAlpha )
{
    calcAlpha = alpha * stackAlpha ;
    
    //Update children
    if ( children.size() > 0 )
    {
        for ( int i = 0 ; i < children.size() ; i++ )
        {
            children[i]->alphaStackUpdate( calcAlpha ) ; 
        }
    }
}

//Return alpha value that plays nice with ofSetColor ( r , g , b , getOFAlpha() ) ;
float ofxAlphaStackItem::getOFAlpha( )
{
    return calcAlpha * 255.0f  ;
}
