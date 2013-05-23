#pragma once

#include "ofMain.h"

class ofxTransitionItem
{
	public :
        ofxTransitionItem( ) ;
        ~ofxTransitionItem( )  { } 

        void transitionIn( float time , float delay ) ;
        void transitionOut( float time , float delay ) ;
        void transitionInComplete( float * args ) ;
        void transitionOutComplete( float * args ) ;


    protected :
		bool bInTransition ;

};
