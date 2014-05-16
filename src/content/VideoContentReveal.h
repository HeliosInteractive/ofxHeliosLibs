#ifndef VIDEOCONTENTREVEAL_H
#define VIDEOCONTENTREVEAL_H


#include <BaseContent.h>
#include "ImagePlus.h"
//#include "ofVideoPlayer_alpha.h"

class VideoContentReveal : public BaseContent
{
    public:
        VideoContentReveal();
        virtual ~VideoContentReveal();

        void setup ( string _path , ofRectangle _bounds )  ;
        void additionalSetup( string _path ) ;
        void draw ( ) ;
        void update () ;

        void inputBegin( ofVec2f input ) ;
        void inputUpdate( ofVec2f input ) ;
        void inputEnd( ofVec2f input ) ;

        ofImage stopImage ;
        ofImage videoFrame ;
        ImagePlus playImage ;

        void setupMap( string path )
        {
            //playImage = ImagePlus ( path , ofVec2f ( 335 , -110 ) , 0 , 0 ) ;
           // playImage.inheritAlpha = true ;
           // playImage.setHitBounds ( ofRectangle ( playImage.x  , playImage..y , 300 , 220 )  ) ;
        }


        void transitionOut( float time = 0.25f, float delay = 0.0f ) ;
        void transitionIn ( float time = 0.25f, float delay = 0.0f ) ;
        void transitionInComplete( float * args ) ;
        void transitionOutComplete ( float * args ) ;
        void initialValues() ;

        int getWidth( ) { return video.getWidth() ; }
        int getHeight( ) { return video.getHeight() ; }


    protected:
          //InteractionTimes
        // float startInteractionTime ;
       //  float scrubberInteractionDelay ;

        ofVideoPlayer video ;
        ofVideoPlayer video2 ;

        ofTexture getTextureReference( )
        {
            if ( video2.getPosition() > 0.0f )
                return video2.getTextureReference( ) ;
            else
                return video.getTextureReference( ) ;
        }


        float scale ;
       // bool isPlaying ;

    private:
};

#endif // VIDEOCONTENTREVEAL_H
