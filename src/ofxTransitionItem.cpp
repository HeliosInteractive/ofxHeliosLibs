//
//  ofxTransitionItem.cpp
//  BE_Aerospace
//
//  Created by Ben McChesney on 3/19/13.
//
//

#include "ofxTransitionItem.h"

ofxTransitionItem::ofxTransitionItem() {
    bInTransition = false ;
} ;

void ofxTransitionItem::transitionIn( float time , float delay )
{
    if ( bInTransition ) return ;
    
    bInTransition = true ;
    
    //Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    //Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &TransitionItem::transitionInComplete ) ;
}

void ofxTransitionItem::transitionOut( float time , float delay )
{
    if ( bInTransition ) return ;
    
    bInTransition = true ;
    
    //Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    //Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &TransitionItem::transitionOutComplete ) ;
    
}

void ofxTransitionItem::transitionInComplete( float * args )
{
    bInTransition = false ;
}

void ofxTransitionItem::transitionOutComplete( float * args )
{
    bInTransition = false ;
}
