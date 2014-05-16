#include "VideoContentReveal.h"

VideoContentReveal::VideoContentReveal()
{
    //ctor
 //    startInteractionTime = -4 ;
 //   scrubberInteractionDelay = 0.25 ;
}

VideoContentReveal::~VideoContentReveal()
{
    //dtor
}

void VideoContentReveal::additionalSetup( string _path )
{
    video2.loadMovie ( _path ) ;
    video2.setAnchorPercent( 0.5 , 0.5 ) ;
    video2.setLoopState( OF_LOOP_NONE ) ;
}

void VideoContentReveal::setup ( string _path , ofRectangle _bounds )
{
    path = ofToDataPath( _path ) ;
    video.loadMovie ( _path ) ;
    video.setAnchorPercent( 0.5 , 0.5 ) ;
    video.setLoopState( OF_LOOP_NONE ) ;



    if ( _path == "/videos/AboutAll.mov" )
        video.setSpeed( 1.25f ) ;

   // cout << "video speed is: " << video.getSpeed() << endl ;
    cout << "attemping to load path" << path << endl ;
   // bool result = video.loadMovie( ofToDataPath( path ) ) ;
   // cout << " result : " << result << endl ;
    //scale = 1 / ((float)video.width  / (float) _bounds.width) ;
    scale = 1.0f ;
  //  cout << "scale : " << scale << " | 1/ ( " << video.getWidth() << " / " << _bounds.width << " )" <<  endl ;
  //  cout << "heights : " << video.getHeight() << " , " << video.height << endl ;
   // float newHeight = (float)video.getHeight() * scale ;
   // float newWidth = (float)video.getWidth() * scale ;
    //bounds = ofRectangle( _bounds.width , _bounds.height  ,newWidth , newHeight ) ;
    //bounds = ofRectangle( 125 , newHeight * -.65 ,newWidth , newHeight ) ;
    initialValues() ;
    maxScale = 1.0f ;
    //In here for Debug
    //video.setVolume(0.0f ) ;
}

void VideoContentReveal::initialValues()
{
    alpha = 0.0f ;
    scale = 1.0f ;
    video.setPosition( 0.0f ) ;
    video.setPaused( true ) ;
    video.stop() ;
}

void VideoContentReveal::draw ( )
{
    if ( alpha == 0 ) return ;
    /*
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    if ()
    mainContent.draw ( 0 , -110 ) ;
    ofFill() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
*/

      ofEnableAlphaBlending() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    //mainContent.draw ( 0 , -108) ;
    //matrixPush () ;
    ofPushMatrix( ) ;
      //  cout << " x  " << x << " y : " << y << endl ;
      //  ofTranslate( x , y , 0 ) ;
            ofScale ( scale , scale ) ;
      //  image.draw ( 0 , 0 ) ;
          if( video.isPlaying() == true )
            {
                video.draw( 0 , 0 ) ;
               // videoFrame.draw (  80 , -110 ) ;
               // stopImage.draw ( 385 , -110 ) ;
            }
            else
            {
               // ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
               // playImage.draw ( 385 , -110 ) ;
            }

            if ( video2.isPlaying() == true )
            {
                video2.draw( 0 , 0 ) ;
            }

            ofPopMatrix( ) ;


    ofFill() ;
    ofSetColor ( 255 , 255 , 255 ) ;

}

void VideoContentReveal::transitionOut( float time , float delay )
{
    Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &scale , scale , 0.01f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &playImage.alpha , playImage.alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    video.setPaused( true ) ;
    video.stop() ;
}

void VideoContentReveal::transitionIn ( float time , float delay )
{
    initialValues() ;
     video.play() ;
     video.update() ;
     video.stop() ;
    Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &scale , scale , maxScale , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &VideoContentReveal::transitionInComplete ) ;
    //Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , VideoContent::transitionInComplete ) ;
//     Tweenzor::add( &playImage.alpha , playImage.alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
}

void VideoContentReveal::transitionInComplete ( float * args )
{
    video.setLoopState( OF_LOOP_NONE ) ;
    video.play() ;

}

void VideoContentReveal::transitionOutComplete ( float * args )
{

}


/*
void VideoContent::inputFromHex ( int hexColor )
{
    if ( hexColor == playImage.getMapHexColor() )
    {
        if ( playImage.timeInputThreshold() == true )
        {
            cout << "time : " << ofGetElapsedTimef() << " >= " << playImage.lastInputTime << " + " << playImage.inputTimeDelay << endl ;
            if ( video.isPlaying() == true )
            {
                cout << "was playing should now be paused" << endl ;
                video.setPaused( true ) ;
                video.stop() ;
            }
            else
            {
                video.setPaused( false ) ;
                video.play( ) ;
                cout << "was paused should now be playing" << endl ;
            }
        }
        else
        {
            cout << "   quit touching me pervert! wait : " << (playImage.lastInputTime + playImage.inputTimeDelay) - ofGetElapsedTimef() << endl ;
        }
       // cout << "input from Hex! isPLaying is now : " << isPlaying << endl ;
    }
}
*/
void VideoContentReveal::inputBegin( ofVec2f input )
{

    cout << "Input begins on video!" << endl ;
    firstInput = true ;

    //if ( video.isPaused() == false )
    if ( video.getIsMovieDone() == true )
    {
        video.setPaused( true ) ;
        video.stop() ;
        video2.play() ;
        cout << "was playing should now be paused" << endl ;
       // video.setPaused( true ) ;
      //  video.stop() ;
    }
    /*
    else
    {
       // video.setPaused( false ) ;
       // video.play( ) ;
        cout << "was paused should now be playing" << endl ;
    }*/

}

void VideoContentReveal::inputUpdate ( ofVec2f input )
{
    cout << "input update!" << endl ;

    //Now a scrubber
}

void VideoContentReveal::update( )
{
    if ( video.isPlaying() )
    {
        video.update() ;
    }

    if ( video2.isPlaying() == true )
    {
        video2.update() ;
    }
}

void VideoContentReveal::inputEnd ( ofVec2f input )
{
   // return ;

    cout << "InputEnd ! " ;
    if ( input.y >= 0.8f && input.y <= 1.0f )
    {
        cout << "hitting the timeline ! " << endl;
    }
    /*
    if ( ofGetElapsedTimef() < startInteractionTime + scrubberInteractionDelay )
    {
        //Play / Pause Movie
        startInteractionTime = -4 ;
        cout << "Play/Pause movie !" << endl ;
        if ( video.isPlaying() == true )
        {
            cout << "was playing should now be paused" << endl ;
            //video.setPaused( true ) ;
            video.stop() ;
        }
        else
        {
           // video.setPaused( false ) ;
            video.play( ) ;
            cout << "was paused should now be playing" << endl ;
        }
    }
    else
    {
        //End of scrubbing
        cout << "Ended a scrubber" << endl ;
    }
    */
    firstInput = false ;

}
