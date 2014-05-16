#ifndef TEXTIMAGECONTENT_H
#define TEXTIMAGECONTENT_H

#include "ofMain.h"
#include "BaseContent.h"

class TextImageContent : public BaseContent
{
    public:
        TextImageContent() { }
        virtual ~TextImageContent() { }

        void update() ;
        void inputUpdate ( ofVec2f input ) ;
        void inputBegin( ofVec2f input ) ;
        void inputEnd( ofVec2f input ) ;

        void setup ( string _path , ofRectangle _bounds ) ;
        void draw ( ) ;

        ofImage image ;

        //Mask using a shader
        ofImage mask;
        ofShader maskShader;

    protected:
        ofVec2f lastInput ;
        ofVec2f startDrag ;
        ofVec2f updateDrag ;
        float dragSpeed ;
        float startScroll ;
        ofRectangle scrollBounds ;
        ofVec2f scroll ;

    private:
        void roundedRect ( float x , float y , float w, float h , float r ) ;
        void quadraticBezierVertex( float cpx , float cpy , float x ,  float y , float prevX , float prevY ) ;


};

#endif // TEXTIMAGECONTENT_H
