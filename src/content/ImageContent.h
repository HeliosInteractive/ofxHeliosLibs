#ifndef IMAGECONTENT_H
#define IMAGECONTENT_H

#include "BaseContent.h"

class ImageContent : public BaseContent
{
    public:
        ImageContent() { }
        virtual ~ImageContent() { }

        void setup ( string _path , ofRectangle _bounds ) ;
        void draw ( ) ;

        void transitionOut( float time = 0.25f, float delay = 0.0f ) ;
        void transitionIn( float time = 0.25f, float delay = 0.0f ) ;

        void transitionIn( ofVec2f _offset , float time = 0.5f , float delay = 0.0f ) ;
		void transitionOut( ofVec2f _offset , float time = 0.5f , float delay = 0.0f ) ;
        void initialValues () ;
        ofImage image ;

        int getWidth() { return image.width ; }
        int getHeight() { return image.height ; }

        ofTexture getTextureReference( )
        {
            return image.getTextureReference( ) ;
        }

        int getType( ) { return 0 ; }

    protected:
    private:
};

#endif // IMAGECONTENT_H
