#include "VideoTimelineContent.h"

VideoTimelineContent::VideoTimelineContent()
{
    //ctor
 //    startInteractionTime = -4 ;
 //   scrubberInteractionDelay = 0.25 ;
}

VideoTimelineContent::~VideoTimelineContent()
{
    //dtor
}

void VideoTimelineContent::additionalSetup( string _path )
{
    cout << " attempting to load : " << _path << endl ;
    videoTimeline.loadMarkerXml( _path ) ;

  //  setType( 3 ) ;
        initialValues() ;
}

void VideoTimelineContent::setup ( string _path , ofRectangle _bounds )
{
    cout << "VIDEO TIMELINE CONTENT ::setup() " << endl;
    path = ofToDataPath( _path ) ;

    videoTimeline.setup( _path ) ;
    videoTimeline.play( ) ;

    cout << "attemping to load path" << path << endl ;
    scale = 1.0f ;

    maxScale = 1.0f ;
    bDebug = false ;


}

void VideoTimelineContent::initialValues()
{
    alpha = 0.0f ;
    scale = 1.0f ;
    videoTimeline.video.setPosition( 0.0f ) ;
    videoTimeline.video.setPaused( true ) ;
    videoTimeline.video.stop() ;
    videoTimeline.currentIcon = 0 ;
    videoTimeline.lastIdleMarkerTime = -2 ;
    videoTimeline.bCheckMarkerStop = true ;

    if ( videoTimeline.markers.size() > 0 )
    {
         Tweenzor::add( &videoTimeline.iconPlayheadX , videoTimeline.iconPlayheadX ,  videoTimeline.icons[ videoTimeline.currentIcon ]->x , 0.25f , 0.55F , EASE_OUT_QUAD ) ;
         for ( int m = 0 ; m < videoTimeline.markers.size() ; m++ )
         {
            videoTimeline.icons[ m ]->bActive = false ;
         }
    }
}

void VideoTimelineContent::draw ( )
{
    if ( alpha == 0 ) return ;

    ofEnableAlphaBlending() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    ofPushMatrix( ) ;

        ofScale ( scale , scale ) ;
        videoTimeline.drawVideo( ) ;

    ofPopMatrix( ) ;

    ofFill() ;
    ofSetColor ( 255 , 255 , 255 ) ;
}

void VideoTimelineContent::drawUI ( )
{
    videoTimeline.drawTimeline( ) ;
}

void VideoTimelineContent::transitionOut( float time , float delay )
{
    Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &scale , scale , 0.01f , delay , time , EASE_OUT_QUAD ) ;
   // Tweenzor::add( &playImage.alpha , playImage.alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
   // video.setPaused( true ) ;
   // video.stop() ;
   videoTimeline.video.setPaused( true ) ;
   videoTimeline.video.stop()  ;
}

void VideoTimelineContent::transitionIn ( float time , float delay )
{
    initialValues() ;
     videoTimeline.video.play() ;
     videoTimeline.video.update() ;
     videoTimeline.video.stop() ;
    Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &scale , scale , maxScale , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &VideoTimelineContent::transitionInComplete ) ;
}

void VideoTimelineContent::transitionInComplete ( float * args )
{
    videoTimeline.video.play ( ) ;

}

void VideoTimelineContent::transitionOutComplete ( float * args )
{

}

void VideoTimelineContent::inputBegin( ofVec2f input )
{

    //cout << "Input begins on video!" << endl ;
    firstInput = true ;

    cout << "VideoTimelineContent:: inputBegin : " << input.x << " , " << input.y << endl ;
    if ( input.x >= .1 && input.x <= 0.9f )
    {
        cout << "X is ok!" << endl ;
        if ( input.y >= 0.75f && input.y <= 0.95f )
        {
            cout << "Y is OK! " << endl ;
            videoTimeline.normalizedinputUp( input.x , input.y ) ;
            cout << "hitting the timeline ! " << endl;
        }
    }
    //videoTimeline.inputDown( input.x , input.y ) ;

/*
    //if ( video.isPaused() == false )
    if ( video.getIsMovieDone() == true )
    {
        video.setPaused( true ) ;
        video.stop() ;
        video2.play() ;
        cout << "was playing should now be paused" << endl ;


    } */
}

void VideoTimelineContent::inputUpdate ( ofVec2f input )
{
    cout << "input update!" << endl ;
   // videoTimeline.inputDrag( input.x , input.y ) ;
}

void VideoTimelineContent::update( )
{
    videoTimeline.bDebugTimeline = bDebug ;
    videoTimeline.update( ) ;
}

void VideoTimelineContent::inputEnd ( ofVec2f input )
{
  //  cout << "InputEnd ! X " << input.x << " , " << input.y << endl ;

    firstInput = false ;
    //videoTimeline.inputUp( input.x , input.y ) ;

}


