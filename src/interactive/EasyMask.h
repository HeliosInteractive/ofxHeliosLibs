#ifndef EASYMASK_H
#define EASYMASK_H

#include "ofMain.h"

class EasyMask
{
    public:
        EasyMask();
        virtual ~EasyMask();

        void setup( string shaderPath , ofRectangle _area ) ;

        //Masking
        ofImage mask ;
        ofRectangle maskArea ;
        ofRectangle originalMaskArea ;
		ofShader maskShader;

        void setMask( string maskPath ) ;
        void setMask( ofImage newMask ) ;

		void drawMask ( ofTexture tex1 , ofTexture tex2 , float xOffset , float yOffset , float contentAlpha ) ;

         static EasyMask* Instance()
         {
             static EasyMask inst ;
             return &inst ;
         }

    protected:
    private:
};

#endif // EASYMASK_H
