#ifndef VIDEOCONTENT_H
#define VIDEOCONTENT_H

#include <BaseContent.h>
#include "ImagePlus.h"
//#include "ofGstVideoPlayer.h"
#include "ofxWMFVideoPlayer.h"
//#include "ofVideoPlayer_alpha.h"

class VideoContent : public BaseContent
{
    public:
        VideoContent();
        virtual ~VideoContent();

        void setup ( string _path , ofRectangle _bounds )  ;
        void draw ( ) ;
        void update () ;

        void setLoop ( bool bDoLoop )
        {
            if ( bDoLoop == true )
                video.setLoopState( OF_LOOP_NORMAL ) ;
            else
                video.setLoopState( OF_LOOP_NONE ) ;
        }

        void loadBGImage ( string path )
        {
            bgImage.loadImage( path ) ;
            bgImage.setAnchorPercent( 0.5 , 0.5 ) ;
        }

        int getType () { return 1 ; }

        ofImage bgImage ;

        void inputBegin( ofVec2f input ) ;
        void inputUpdate( ofVec2f input ) ;
        void inputEnd( ofVec2f input ) ;

        ofImage stopImage ;
        ofImage videoFrame ;
        ImagePlus playImage ;

        bool updateFrameOnce ;

        bool bUseFboTexture ;
        ofFbo altTexture ;
        void createAltTexture( ) ;

		ofFbo videoFbo ; 
        ofTexture getTextureReference( )
        {
            if ( bUseFboTexture == false )
                return videoFbo.getTextureReference( ) ;
            else
                return altTexture.getTextureReference( ) ;
        }


		void drawIntoTexture( ) ; 

        void transitionOut( float time = 0.25f, float delay = 0.0f ) ;
        void transitionIn ( float time = 0.25f, float delay = 0.0f ) ;
        void transitionInComplete( float * args ) ;
        void transitionOutComplete ( float * args ) ;
        void initialValues() ;

        int getWidth( );
        int getHeight( ) ;
		//void unload( ) ; 

        ofxWMFVideoPlayer video ;


    protected:
       // InteractionTimes
       // float startInteractionTime ;
       // float scrubberInteractionDelay ;



        float scale ;
       // bool isPlaying ;

    private:
};

#endif // VIDEOCONTENT_H
