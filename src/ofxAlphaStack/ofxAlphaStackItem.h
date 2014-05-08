#ifndef ofxAlphaStackItem_H
#define ofxAlphaStackItem_H


#include "ofMain.h"
class ofxAlphaStackItem
{
    public:
        ofxAlphaStackItem( ) { }
        ~ofxAlphaStackItem() { }

        void setup ( ) ; 
        //Most basic update multiplies stackAlpha by current alpha
        //Override this and pass along to children is need be
        virtual void alphaStackUpdate ( float stackAlpha ) ;
    
        //Return alpha value that plays nice with ofSetColor ( ofColor color , getOFAlpha() ) ;
        float getOFAlpha( ) ;

        float alpha ;       //Alpha works similar to inside flash : a float from 0 - 1
        float calcAlpha ;   //Calc Alpha is the multplied alpha for this item where it is on teh stack
    
        vector < ofxAlphaStackItem * > children ; 

		string getDebugString() ; 
		void drawChildren( ) ; 
		virtual void draw( ) { }  
    

    protected:
    private:
};

#endif // ofxAlphaStackItem_H
