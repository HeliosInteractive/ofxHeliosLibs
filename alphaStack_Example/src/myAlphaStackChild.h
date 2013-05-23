#ifndef MYALPHASTACKCHILD_H
#define MYALPHASTACKCHILD_H

#include "ofMain.h"
#include "AlphaStackItem.h"

class myAlphaStackChild : public AlphaStackItem
{
    public:
        myAlphaStackChild() { }

        float x , y ;

        void alphaStackUpdate ( float stackAlpha )
        {
            calcAlpha = alpha * stackAlpha ;
        }

        void setup ( )
        {
            color = ofColor ( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ) ;
            calcAlpha = 1.0f ;
            alpha = 1.0f ;
        }

        void draw( )
        {
            ofSetColor( color.r , color.g , color.b , calcAlpha * 255.0f ) ;
            ofRect ( x , y  , 20 , 20 ) ;
        }

        void update( )
        {

        }

        ofColor color ;
        float alpha ;
        float calcAlpha ;
    protected:
    private:
};

#endif // MYALPHASTACKCHILD_H
