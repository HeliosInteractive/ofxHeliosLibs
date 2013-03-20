#ifndef MYALPHASTACKITEM_H
#define MYALPHASTACKITEM_H

#include "ofMain.h"
#include "AlphaStackItem.h"
#include "myAlphaStackChild.h"

class myAlphaStackItem : public AlphaStackItem
{
    public:
        myAlphaStackItem() { }

        float x , y ;
        vector<myAlphaStackChild> children ;

        void setup ( )
        {
            color = ofColor ( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ) ;
            calcAlpha = 1.0f ;
            alpha = 1.0f ;

            for ( int i = 0 ; i < 3 ; i++ )
            {
                myAlphaStackChild child ;
                ofTranslate( x , y , 0 ) ;
                 ofRect ( 0 , 0 , 120 , 120 ) ;
            for ( int i = 0 ; i < children.size() ; i++ )
            {
                children[i].draw ( ) ;
            }

            ofPopMatrix( ) ;

        }

        ofColor color ;
        float alpha ;
        float calcAlpha  ;

    protected:
    private:
};

#endif // MYALPHASTACKITEM_H
