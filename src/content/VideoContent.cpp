#include "VideoContent.h"
#include "EasyMask.h"

VideoContent::VideoContent()
{
 
}

VideoContent::~VideoContent()
{
    //dtor
}

void VideoContent::setup ( string _path , ofRectangle _bounds )
{
    path = ofToDataPath( _path ) ;

    scale = 1.0f ;
    initialValues() ;
    maxScale = 1.0f ;

    updateFrameOnce = false ;
    bUseFboTexture = false ;


}

void VideoContent::initialValues()
{
    alpha = 0.0f ;
    scale = 1.0f ;
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
      
        video.draw( offset.x , offset.y ) ;
        video.stop() ;
        ofPopMatrix( ) ;
    altTexture.end() ;
}

void VideoContent::drawIntoTexture( ) 
{
	videoFbo.begin() ; 
		ofSetColor( 255 ) ; 
		video.draw( 0 , 0 ) ;
	videoFbo.end() ; 
}

int VideoContent::getWidth( ) 
{
	return video.getWidth() ; 
}

int VideoContent::getHeight( )
{
	return video.getHeight() ; 
}

void VideoContent::draw ( )
{
    if ( alpha == 0 ) return ;
    
	videoFbo.begin() ; 
		ofSetColor( 255 ) ;
		video.draw( 0 , 0 ) ; 
	videoFbo.end() ; 

    ofPushMatrix( ) ;
 
        ofScale ( scale , scale ) ;

          if( video.isPlaying() == true )
            {

                video.draw( offset.x , offset.y ) ;
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
    initialValues() ;
	video.loadMovie( path ) ; 
    updateFrameOnce = true ;
    
	video.play() ; 
	videoFbo.allocate( video.getWidth() , video.getHeight() ) ; 
	
	videoFbo.begin() ; 
		ofClear( 0 , 0 , 0, 1 ) ; 
		drawIntoTexture() ; 
	videoFbo.end() ; 


    Tweenzor::add( &alpha , 0.0f , 1.0f , delay , time , EASE_OUT_QUAD ) ;
	Tweenzor::addCompleteListener( Tweenzor::getTween( &alpha ) , this , &VideoContent::transitionInComplete ) ;
}

void VideoContent::transitionInComplete ( float * args )
{
	video.play() ; 
    video.setLoopState( OF_LOOP_NONE ) ;

}

void VideoContent::transitionOutComplete ( float * args )
{
  

}

void VideoContent::inputBegin( ofVec2f input )
{

  //  cout << "Input begins on video!" << endl ;
    firstInput = true ;

    if ( video.getIsMovieDone() )
    {
        //setLoop( true );
//        video.firstFrame() ;
       // video.setPosition( 0.0f ) ;
        //video.setPaused( false ) ;
        video.play( ) ;
        video.update() ;
        //setLoop( false );
        //cout << "start over! " << endl ;

        return ;
    }

    return ;
}

void VideoContent::inputUpdate ( ofVec2f input )
{
   // cout << "input update!" << endl ;

    //Now a scrubber
}

void VideoContent::update( )
{
	video.update() ;
    if ( video.isPlaying() )
    {
       
        if( updateFrameOnce == true )
        {
            updateFrameOnce = false ;
           // video.stop() ;
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
