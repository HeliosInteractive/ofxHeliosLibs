#ifndef IMAGEGALLERYCONTENT_H
#define IMAGEGALLERYCONTENT_H

#include <BaseContent.h>
#include "ImagePlus.h"

class ImageGalleryContent : public BaseContent
{
    public:
        ImageGalleryContent();
        virtual ~ImageGalleryContent();

        void update() ;
        void inputUpdate ( ofVec2f input ) ;
        void inputBegin( ofVec2f input ) ;
        void inputEnd( ofVec2f input ) ;

        void setup ( string _path , ofRectangle _bounds ) ;

        void draw ( ) ;
        void drawMap ( ) ;

        void setupArrows ( string leftPath , string rightPath , string leftSelected , string rightSelected ) ;
        ImagePlus next ;
        ImagePlus prev ;
        ImagePlus nextSelected ;
        ImagePlus prevSelected ;

        void setupPagination ( string fillPath , string noFillPath ) ;
        ofImage fillIndicator ;
        ofImage noFillIndicator ;

        void transitionOut( float time = 0.25f, float delay = 0.0f ) ;
        void transitionIn( float time = 0.25f, float delay = 0.0f ) ;
        void transitionComplete( float * args ) ;
        void initialValues( ) ;


        //manage the transitions
        bool inTransition ;

        int getWidth() { return gallery[0].image.width ; }
        int getHeight() { return gallery[0].image.height ; }

        void drawUI ( ) ;

         float getXOffset ( ) { return galleryXOffset ; }
        ofTexture getTextureReference()
        {
            if ( focusGallery > -1 )
                return gallery[focusGallery].image.getTextureReference() ;
            else
                return ofTexture() ;
        }

    protected:
        ofDirectory dir ;
        vector<ImagePlus> gallery ;
        ofVec2f lastInput ;
        float galleryXOffset ;
        int focusGallery ;






    private:
};

#endif // IMAGEGALLERYCONTENT_H
