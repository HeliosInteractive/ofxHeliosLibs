#ifndef HITRECTANGLE_H
#define HITRECTANGLE_H

#include "ofMain.h"

class HitRectangle
{
    public:
        HitRectangle()
        {
            bounds = ofRectangle ( 0 , 0 , 2 , 2 ) ;
            debugColor = ofColor ( ofRandom( 255 ) , ofRandom ( 255 ) , ofRandom( 255 ) ) ;
        }
        virtual ~HitRectangle() {}

        ofRectangle bounds ;
        void setBounds ( ofRectangle _b )
        {
            bounds = _b ;
        }

        void setBounds ( float x , float y , float w , float h )
        {
            bounds = ofRectangle ( x , y , w , h ) ;
        }

        ofRectangle getBounds ( )
        {
            return bounds ;
        }

        void draw ( )
        {
            ofSetColor ( 255 , 255 , 255 , 125 ) ;
            ofRect ( bounds ) ;
        }

        bool hitTest( ofVec2f p )
        {
            return (( p.x > bounds.x ) && ( p.x < bounds.x + bounds.width ) && ( p.y > bounds.y ) && ( p.y < bounds.y + bounds.height ) ) ;
        }

        void setDebugColor ( ofColor _dColor ) { debugColor = _dColor ; }
        ofColor getDebugColor ( ) { return debugColor ; }

    protected:
        ofColor debugColor ;

    private:
};

#endif // HITRECTANGLE_H
