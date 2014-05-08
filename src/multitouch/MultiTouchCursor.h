#ifndef MULTITOUCHCURSOR_H
#define MULTITOUCHCURSOR_H

#include "ofMain.h"

/*
    November 2011  written by Ben McChesney of Helios Interactive
    This class is meant to be a general wrapper for touch so as not to be solely reliant on TUIO
*/

class MultiTouchCursor
{
    public:
        MultiTouchCursor();
        virtual ~MultiTouchCursor();

        ofPoint position, speed ;
        int id ;

		void setup ( int _id  , ofPoint p  ) ;
        void setup ( int _id  , float x , float y , float z  = 0 ) ;
        void update ( ) ;
        void draw ( ) ;

        float getX( ) { return position.x  ; }
        float getY( ) { return position.y ; }
		float getZ( ) { return position.z ; } 
        int getCursorID( ) { return id ; }

        void onDown ( ofPoint p ) ;
        void onDrag( ofPoint p ) ;
        void onUp ( ofPoint p ) ;

        bool bDrag ;

    protected:
    private:
};

#endif // MULTITOUCHCURSOR_H
