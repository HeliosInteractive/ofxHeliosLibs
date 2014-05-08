#ifndef MULTITOUCHOBJECT_H
#define MULTITOUCHOBJECT_H

#include "ofxMatrixTransformObject.h"
#include "MultiTouchActions.h"

class MultiTouchObject : public ofxMatrixTransformObject
{
    public:
        MultiTouchObject();
        virtual ~MultiTouchObject();

        void setup ( float x , float y , float size , float dampen ) ;
        void update ( ) ;
        void draw ( ) ;

		ofPoint getPosition( ) { return ofPoint ( x , y ) ; } 

        ofRectangle area ;
        ofColor color ;

        MultiTouchActions multiTouch ; 
        float dampen ;

		string getDebugOutput() ; 


    protected:
    private:
};

#endif // MULTITOUCHOBJECT_H
