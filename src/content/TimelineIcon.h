#ifndef TimelineIcon___H
#define TimelineIcon___H

#include "ofMain.h"
#include "ofxTextSuite.h"
#include "ColorPalette.h"

class TimelineIcon
{
    public:
        TimelineIcon() {}
        virtual ~TimelineIcon() {}
        float x , y ;
        ofRectangle bounds ;
        ofImage image ;
        ofxTextBlock labelBlock ;
        bool bActive ;


        void setup ( float _x , float _y , string imagePath , string label )
        {
            bActive = false ;
            x = _x ;
            y = _y ;
            image.loadImage( imagePath ) ;
            image.setAnchorPercent( 0.5 , 0.5 ) ;
            bounds = ofRectangle( x + ( image.width/-2 ) , y + ( image.height/-2)  , image.width , image.height ) ;
            cout<< " x : " << bounds.x << " ,  y : " << bounds.y << " W " << image.width << " , h : " << image.height << endl ;
            labelBlock.init ( "type/Whitney-Book-Bas.otf" , 15 , 1.0f ) ;
            labelBlock.setText( label ) ;
        }

        void update ( )
        {
            //labels.push_back( "SECONDS" ) ;
        }

        void draw ( )
        {
            ofSetColor( 255 , 255 , 255 ) ;
            //image.draw( bounds.x , bounds.y , bounds.width/4.0f , bounds.height/4.0f ) ;
            ofColor gold = ofColor( 255 , 160 , 0 ) ;
            if ( bActive == false )
                ofSetColor( 255 , 255 , 255 , 215 ) ;
            else
                ofSetColor( gold , 255 ) ;
            labelBlock.drawCenter( bounds.x , bounds.y ) ;
        }

    protected:
    private:
};

#endif // TimelineIcon__H
