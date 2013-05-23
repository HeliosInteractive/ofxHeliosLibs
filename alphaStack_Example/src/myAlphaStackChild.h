#ifndef MYALPHASTACKCHILD_H
#define MYALPHASTACKCHILD_H

#include "ofMain.h"
#include "ofxAlphaStackItem.h"

class myAlphaStackChild : public ofxAlphaStackItem
{
    public:
        myAlphaStackChild() { }

       
        void setup ( float _x , float _y ) ; 
        void draw( ) ; 
        void update( ) ; 

		float x , y ;
        ofColor color ;

    protected:
    private:
};

#endif // MYALPHASTACKCHILD_H
