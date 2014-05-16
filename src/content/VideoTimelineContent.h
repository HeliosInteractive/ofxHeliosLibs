#pragma once

#include <BaseContent.h>
#include "ImagePlus.h"
#include "ofMain.h"
#include "VideoTimeline.h"

class VideoTimelineContent : public BaseContent 
{
    public:
        VideoTimelineContent();
        virtual ~VideoTimelineContent();

        void setup ( string _path , ofRectangle _bounds )  ;
        void additionalSetup( string _path ) ;
        void draw ( ) ;
        void drawUI( ) ;
        void update () ;

        void inputBegin( ofVec2f input ) ;
        void inputUpdate( ofVec2f input ) ;
        void inputEnd( ofVec2f input ) ;

        void transitionOut( float time = 0.25f, float delay = 0.0f ) ;
        void transitionIn ( float time = 0.25f, float delay = 0.0f ) ;
        void transitionInComplete( float * args ) ;
        void transitionOutComplete ( float * args ) ;
        void initialValues() ;

        int getWidth( ) { return videoTimeline.video.getWidth() ; }
        int getHeight( ) { return videoTimeline.video.getHeight() ; }

        VideoTimeline videoTimeline ;

        virtual int getType( )
        {
            return 3 ;
        }

		void normalizedInputUp( float x , float y ) { } 
		

    protected:


        ofTexture getTextureReference( )
        {
            return videoTimeline.video.getTextureReference( ) ;
        }


        float scale ;

    protected:
    private:
};

