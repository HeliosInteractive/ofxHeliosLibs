#ifndef ALPHASTACKITEM_H
#define ALPHASTACKITEM_H


#include "ofMain.h"
class AlphaStackItem
{
    public:
        AlphaStackItem( )
        {
            alpha = 1.0f;
            calcAlpha = 1.0f ;
        }

        virtual ~AlphaStackItem() { }

        //Most basic update multiplies stackAlpha by current alpha
        //Override this and pass along to children is need be
        virtual void alphaStackUpdate ( float stackAlpha )
        {
            calcAlpha = alpha * stackAlpha ;
        }

        //Return alpha value that plays nice with ofSetColor ( r , g , b , getOFAlpha() ) ;
        float getOFAlpha( ) { return calcAlpha * 255.0f  ; }

        float alpha ;       //Alpha works similar to inside flash : a float from 0 - 1
        float calcAlpha ;   //Calc Alpha is the multplied alpha for this item where it is on teh stack

    protected:
    private:
};

#endif // ALPHASTACKITEM_H
