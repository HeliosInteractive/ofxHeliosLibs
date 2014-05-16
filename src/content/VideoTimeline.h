#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "TimelineMarker.h"
#include "TimelineIcon.h"
#include "Tweenzor.h"

//#include "IdleCoordinator.h"

class VideoTimeline
{
    public:
        VideoTimeline();
        virtual ~VideoTimeline();

        //Basic 3 OF functions
        void setup ( string path ) ;
        void update( ) ;
        void drawVideo ( ) ;
        void drawTimeline( ) ;

        //Our video and our debug track
        ofVideoPlayer video ;

        ofRectangle trackBounds ;
        bool bIsDragging ;
        bool bDebugTimeline ;
        float tweenVideoPosition ;
        void videoTweenEnd( float * args ) ;

        void play( ) ;
        void snap ( ) ;
        void togglePlay( ) ;

        ofPoint offset ;
        void inputDown ( float x , float y ) ;
        void inputDrag ( float x , float y ) ;
        void inputUp ( float x , float y ) ;

        void normalizedinputUp ( float x , float y ) ;

        bool hitTest ( float x , float y ) ;
        int hitMarkerTest( float _x , float _y ) ;

        vector<TimelineMarker*> markers ;
        void loadMarkerXml ( string xmlPath ) ;
        void saveMarkerXml ( ) ;

        void addNewMarker( float pos , float outPosition ) ;
        void removeMarkerAt( int markerIndex ) ;
        int activeDragMarkerIndex ;
        vector<TimelineIcon*> icons ;

        float iconX ;
        int currentIcon ;

        bool bCheckMarkerStop ;
        float lastIdleMarkerTime ;
        float idleMarkerDelay ;

        string xmlPath ;

        ofImage sliderTrack ;
        ofImage iconPlayhead ;

        float iconPlayheadX ;

    protected:
    private:
};

#endif // VIDEOTIMELINE_H
