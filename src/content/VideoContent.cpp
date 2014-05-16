#include "VideoContent.h"
#include "EasyMask.h"

VideoContent::VideoContent()
{
    //ctor
 //    startInteractionTime = -4 ;
 //   scrubberInteractionDelay = 0.25 ;
}

VideoContent::~VideoContent()
{
    //dtor
}

void VideoContent::setup ( string _path , ofRectangle _bounds )
{
    // player.setPlayer(ofPtr<ofBaseVideoPlayer>(new ofGstVideoPlayer));
   // video.setPlayer( ofPtr<ofBaseVideoPlayer>(new ofGstVideoPlayer) ) ;
    path = ofToDataPath( _path ) ;
   // video.setPixelFormat( OF_PIXELS_RGB ) ;
    video.loadMovie ( _path ) ;
  //  video.firstFrame() ;
    video.setAnchorPercent( 0.5 , 0.5 ) ;
    //video.setLoopState( OF_LOOP_NONE ) ;

    if ( _path == "/videos/AboutAll.mov" )
        video.setSpeed( 1.25f ) ;

   // cout << "video speed is: " << video.getSpeed() << endl ;
  //  cout << "attemping to load path" << path << endl ;
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

    updateFrameOnce = false ;
    bUseFboTexture = false ;
    //In here for Debug
    //video.setVolume(0.0f ) ;
}

void VideoContent::initialValues()
{
   //cout << "video content intiial values found!" << endl ;
    alpha = 0.0f ;
    scale = 1.0f ;
   // video.play() ;
    video.setPosition( 0.0f ) ;
   // video.firstFrame() ;
    video.play() ;
    video.update( ) ;
    //video.setPaused( true ) ;
    video.stop() ;
}

void VideoContent::createAltTexture( )
{
    bUseFboTexture = true ;
    ofRectangle texArea = EasyMask::Instance()->maskArea ;
    altTexture.allocate( texArea.width , texArea.height , GL_RGBA ) ;
    altTexture.begin() ;
        ofClear( 0 , 0 , 0 , 1 ) ;
    altTexture.end() ;


    altTexture.begin() ;
    ofPushMatrix( ) ;
        ofTranslate( bgImage.width / 2 , bgImage.height / 2 ) ;
        bgImage.draw( 0 , 0 ) ;
       // video.setPosition( 0.1 ) ;
       // video.play() ;
       // video.update( ) ;
        video.draw( offset.x , offset.y ) ;
        video.stop() ;
        ofPopMatrix( ) ;
    altTexture.end() ;
}

void VideoContent::draw ( )
{
    if ( alpha == 0 ) return ;
    /*
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    if ()
    mainContent.draw ( 0 , -110 ) ;
    ofFill() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
*/

   //   ofEnableAlphaBlending() ;
    //ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    //mainContent.draw ( 0 , -108) ;
    //matrixPush () ;
    ofPushMatrix( ) ;
      //  cout << " x  " << x << " y : " << y << endl ;
      //  ofTranslate( x , y , 0 ) ;
        ofScale ( scale , scale ) ;
       // if ( bgImage.isAllocated() )
       //     bgImage.draw( 0 , 0 ) ;
      //  image.draw ( 0 , 0 ) ;
          if( video.isPlaying() == true )
            {

                video.draw( offset.x , offset.y ) ;

               // videoFrame.draw (  80 , -110 ) ;
               // stopImage.draw ( 385 , -110 ) ;
            }
            else
            {
               // ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
               // playImage.draw ( 385 , -110 ) ;
            }

            ofPopMatrix( ) ;


    ofFill() ;
    ofSetColor ( 255 , 255 , 255 ) ;

}

void VideoContent::transitionOut( float time , float delay )
{
    /*
    //Use these bools to make transitions more modular
    if ( bAlpha )
    {
        Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
        Tweenzor::add( &playImage.alpha , playImage.alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
        Tweenzor::addCompleteListener( Tweenzor::getTween( &playImage.alpha ) , this , &VideoContent::transitionOutComplete ) ;
    }
    if ( bScale )
        Tweenzor::add( &scale , scale , 0.01f , delay , time , EASE_OUT_QUAD ) ;
*/
    video.setPaused( true ) ;
    video.stop() ;

    if ( !bAlpha && !bScale && !bTranslate )
    {
        transitionOutComplete( &scale ) ;
    }
}

void VideoContent::transitionIn ( float time , float delay )
{

    //  setLoop( false ) ;
     initialValues() ;
    //cout << "video transtionIn! " << endl ;
    video.setPosition( 0 ) ;
    video.play() ;
    //video.update() ;
    //update() ;
    updateFrameOnce = true ;
    //video.stop() ;
   // video.setLoopState( OF_LOOP_NONE ) ;
    //video.setPaused( true ) ;
    //video.stop() ;
     //video.play() ;
     //video.update( );
     //video.stop() ;
  //  video.firstFrame( ) ;
  //  video.update( ) ;
  //  video.stop() ;
    // if ( bAlpha && alpha != 1.0f )
    // {

         Tweenzor::add( &alpha , 0.0f , 1.0f , delay , time , EASE_OUT_QUAD ) ;
        Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &VideoContent::transitionInComplete ) ;
     //}
     //else
    // {
      //   transitionInComplete( &alpha ) ;
     //}
    //transitionInComplete( &alpha ) ;
   // if ( bScale )
   //     Tweenzor::add( &scale , scale , maxScale , delay , time , EASE_OUT_QUAD ) ;

    //Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , VideoContent::transitionInComplete ) ;
//     Tweenzor::add( &playImage.alpha , playImage.alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
}

void VideoContent::transitionInComplete ( float * args )
{
    //cout << "video transitionined in complete !" << endl ;
    //video.firstFrame( ) ;
   // video.setPosition( 0 ) ;
    //video.idleMovie( );
    video.play() ;

   // video.stop() ;
    video.setLoopState( OF_LOOP_NONE ) ;

}

void VideoContent::transitionOutComplete ( float * args )
{
  //  cout << "transitionout compelte ! " << endl ;
   video.setPosition( 0 ) ;
   video.update( ) ;
   video.stop( ) ;

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
void VideoContent::inputBegin( ofVec2f input )
{

  //  cout << "Input begins on video!" << endl ;
    firstInput = true ;

    if ( video.getIsMovieDone() )
    {
        //setLoop( true );
        video.firstFrame() ;
        video.setPosition( 0.0f ) ;
        //video.setPaused( false ) ;
        video.play( ) ;
        video.update() ;
        //setLoop( false );
        //cout << "start over! " << endl ;

        return ;
    }

    return ;
    if ( video.isPaused() == false )
    {
        //cout << "was playing should now be paused" << endl ;
        video.setPaused( true ) ;
      //  video.stop() ;
    }
    else
    {
       // video.setPaused( false ) ;
        video.play( ) ;
        //cout << "was paused should now be playing" << endl ;
    }

}

void VideoContent::inputUpdate ( ofVec2f input )
{
   // cout << "input update!" << endl ;

    //Now a scrubber
}

void VideoContent::update( )
{
    if ( video.isPlaying() )
    {
        video.update() ;
        if( updateFrameOnce == true )
        {
            updateFrameOnce = false ;
            video.stop() ;
            //cout << "update frame once called !" << endl ;
        }
       // cout << "video is updating !" << endl ;
    }
}

void VideoContent::inputEnd ( ofVec2f input )
{
   // return ;
    //cout << "InputEnd ! " ;
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
