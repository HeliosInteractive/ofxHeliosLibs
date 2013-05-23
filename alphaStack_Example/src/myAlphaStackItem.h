#ifndef MYALPHASTACKITEM_H
#define MYALPHASTACKITEM_H

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "myAlphaStackChild.h"

class myAlphaStackItem : public ofxAlphaStackItem
{
    public:
        myAlphaStackItem() { }

        float x , y ;
        vector<myAlphaStackChild> children ;

        void setup ( float _x , float _y , int _numChildren ) ; 
		void update( ) ; 
		void draw ( ) ; 

        ofColor color ;

    protected:
    private:
};

#endif // MYALPHASTACKITEM_H
