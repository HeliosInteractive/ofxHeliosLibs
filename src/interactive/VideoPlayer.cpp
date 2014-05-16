//
//  VideoPlayer.cpp
//  emptyExample
//
//  Created by Ben McChesney on 3/28/13.
//
//

#include "VideoPlayer.h"

void VideoPlayer::setup ( ofRectangle bounds )
{
    path = "" ;
	contentRect = bounds ; 
    alpha = 0.0 ;
	//bEventAdded = false ; 
	float seekAreaPadding = 25 ; 
	float seekX = playButton.image.getWidth() + playButton.x + seekAreaPadding ; 
	seekBarArea = ofRectangle( seekX , contentRect.height - 50 , contentRect.getWidth() - seekX - seekAreaPadding  , 50 )  ;
	children.push_back( &playButton ) ; 
	children.push_back( &pauseButton ) ;
	children.push_back( &closeButton ) ; 
//	children.push_back( &timeline ) ; 
	children.push_back( &controlsBG ) ;

	bIsDragScrubbing = false ; 
}

void VideoPlayer::inputDown( float x , float y )
{
	
    if ( playButton.alpha == 1.0f && playButton.hitTest( x , y ) )
    {
        video.play() ;
		ofSetFrameRate( 30 ) ; 
        pauseButton.alpha = 0.0f ;
        playButton.alpha = 1.0f ;

		Tweenzor::add( &playButton.alpha , playButton.alpha , 0.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 
		Tweenzor::add( &pauseButton.alpha , pauseButton.alpha , 1.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 

    }
    if ( pauseButton.alpha == 1.0f && pauseButton.hitTest( x , y ) )
    {
        if ( video.isPlaying() )
        {
			video.setPaused( true ) ; 
            video.previousFrame() ;
			Tweenzor::add( &playButton.alpha , playButton.alpha , 1.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 
			Tweenzor::add( &pauseButton.alpha , pauseButton.alpha , 0.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ;
        }
    }
    if ( closeButton.alpha == 1.0f && closeButton.hitTest( x , y ) )
    {
        stop() ;
    }

	if ( seekBarArea.inside( x ,y ) == true ) 
	{
		bIsDragScrubbing = true ; 
	}
}

void VideoPlayer::inputDrag( float x , float y )
{
	if ( bIsDragScrubbing == true  ) 
	{
		if ( video.isPlaying() == true || video.isPaused() == false  ) 
		{
			video.setPaused( true ) ;
			Tweenzor::add( &playButton.alpha , playButton.alpha , 1.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 
			Tweenzor::add( &pauseButton.alpha , pauseButton.alpha , 0.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ;
		}
		float position = ofMap( x , seekBarArea.x , seekBarArea.x + seekBarArea.getWidth() , 0.0f , 1.0f , true ) ; 
		scrubPosition = position ; 
		startThread( false, false ) ; 
		/*
		if ( position != video.getPosition() ) 
		{
			video.setPosition( position ) ; 
			video.update( ) ; 
		}*/
	}
}

void VideoPlayer::threadedFunction( ) 
{
	//if ( lock() ) 
	//{
		video.setPosition( scrubPosition ) ; 
		//unlock() ; 
		stopThread( ) ; 
	//}
}

void VideoPlayer::inputRelease( float x , float y )
{
	if ( bIsDragScrubbing == true ) 
	{
		video.play( ) ; 
		Tweenzor::add( &playButton.alpha , playButton.alpha , 0.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 
		Tweenzor::add( &pauseButton.alpha , pauseButton.alpha , 1.0f , 0.0f , 0.25f , EASE_OUT_SINE ) ; 
	}
	bIsDragScrubbing = false ; 

	
}

void VideoPlayer::update ( )
{
	if ( !video.isLoaded() || alpha == 0 ) return ; 
   
    video.update() ;
	videoTexture.loadData( video.getPixelsRef() ) ; 
    

	if ( video.isLoaded() == true && video.getPosition() >= 0.98f )
    {
        stop() ;
    }
}

void VideoPlayer::draw ( )
{
	
	if ( alpha == 0 ) return ;
	ofPushStyle() ; 
		ofEnableAlphaBlending( ) ; 

		ofPushMatrix( ) ; 
			ofTranslate( contentRect.x, contentRect.y ) ; 

			float scale = (float)videoTexture.getWidth() / (float)contentRect.getWidth() ;

			ofSetColor( 255 , getOFAlpha() ) ;
			//video.draw( 0 , 0 , contentRect.getWidth() , contentRect.getHeight() );
			videoTexture.draw( 0 , 0 , contentRect.getWidth() , contentRect.getHeight() ) ; 
	
			controlsBG.draw( ) ;
			//controlsBG.draw( ) ;
			//controlsBG.draw( ) ;
			//timeline.draw( ) ; 
			
			/*
			Timeline 
			4 wide , 20 tall , every 10 pixels
			30% white 
*/
			float midSeekY = ofGetHeight() - controlsBG.image.getHeight()/2 ; 
			ofPushMatrix() ; 
				ofTranslate( seekBarArea.x , 0 ) ; 
				ofSetColor( 255.0f * 0.5f , controlsBG.getOFAlpha() ) ;
				int tickY = midSeekY; // seekBarArea.getHeight() / 2 ; 
				int tSpacing = 10 ; 
				int tHeight = 10 ; 
				int tWidth = 4 ; 
				int numTicks = floor( seekBarArea.getWidth() / tSpacing ) - 1 ; 
				
				for ( int i = 0; i < numTicks ; i++ ) 
				{
					ofRect( i * tSpacing , (int) (tickY - floor( tHeight/2 ) + 1 ) , tWidth, (int) tHeight  ) ;
				}
			ofPopMatrix() ;
			//ofSetColor( 255 , videoAlpha * 255.0f ) ;
			playButton.draw( ) ;
			pauseButton.draw( ) ;
			//resetButton.draw( ) ;
			closeButton.draw( ) ;

			//ofSetColor( 0 , (110.0f / 255.0f) * getOFAlpha() ) ;
			//ofRect( 0 , contentRect.height - 50 , contentRect.width , 50 ) ;

			float normalizedWidth = 0 ; 
			if ( video.isLoaded() ) 
				normalizedWidth = video.getPosition() * seekBarArea.getWidth() ; 
			float normalizedX = seekBarArea.x + normalizedWidth ;
			float radius = 15 ; 

			ofSetColor( 43 , 146 , 179 , getOFAlpha() ) ; // * 0.75f ) ;
			ofPushStyle() ;
				ofSetRectMode( OF_RECTMODE_CORNER ) ; 
				float barHeight = tHeight ; //radius * .5 + 2 ; 
				ofRect( seekBarArea.x , midSeekY  - barHeight / 2 , normalizedWidth , barHeight  ) ; 

				ofSetRectMode( OF_RECTMODE_CORNER ) ;
				ofEnableSmoothing() ; 
			
				ofSetColor( 43 , 146 , 179 , getOFAlpha() ) ;
				ofCircle(  normalizedX , midSeekY , radius ) ; 
				ofNoFill() ; 
				ofSetColor( 255 , getOFAlpha() ) ;
				ofSetLineWidth( 2 ) ; 
				ofEnableSmoothing() ; 
				ofCircle(  normalizedX , midSeekY  , radius ) ; 
 
			ofPopStyle() ; 

		ofPopMatrix() ;
	ofPopStyle() ; 
}

void VideoPlayer::stopCompleteHandler( float * args )
{
    video.close() ;
}

void VideoPlayer::playMovie( string _path )
{
	path = _path; 

	videoTexture.clear(); 

    video.setPixelFormat( OF_PIXELS_RGB ) ;
    //ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY ;
    ofLog( OF_LOG_VERBOSE , " about to load video : " + path ) ;
    bool bResult = video.loadMovie( path ) ; //, decodeMode ) ;
	if ( bResult ) 
	{
		ofLogNotice( "VIDEO LOADED OK!" ) ; 
	}
	else
	{
		ofLogError( path ) << " did not load ! " ; 
		return ; 
	}
	video.setUseTexture( false ) ; 
    video.play( ) ;
    //ofLog( OF_LOG_VERBOSE , " video loaded ! and now playing... " ) ;
    video.setLoopState( OF_LOOP_NONE ) ;
	videoTexture.allocate(  video.getWidth() , video.getHeight() , GL_RGB , true ) ; 
	//ofPixels pix( video.getWidth() , video.getHeight() , OF_IMAGE_COLOR ) ; 

	float inc = 0.3f ;
	Tweenzor::add( &alpha , 0.0f , 1.0f , 0.0f , inc , EASE_OUT_QUAD ) ;

	closeButton.alpha = 0 ; 
	//resetButton.alpha = 0 ; 
	pauseButton.alpha = 0 ; 
	playButton.alpha = 0 ; 

	Tweenzor::add( &closeButton.alpha , 0.0f , 1.0f , inc , inc , EASE_OUT_QUAD ) ;
	//Tweenzor::add( &resetButton.alpha , 0.0f , 1.0f , inc * 2.0f , inc , EASE_OUT_QUAD ) ;
	Tweenzor::add( &pauseButton.alpha , 0.0f , 1.0f , inc * 3.0f , inc , EASE_OUT_QUAD ) ;
	Tweenzor::add( &controlsBG.alpha  , 0.0f , 1.0f , inc * 1.0f , inc , EASE_OUT_QUAD ) ;
	//Tweenzor::add( &timeline.alpha  , 0.0f , 1.0f , inc * 2.0f , inc , EASE_OUT_QUAD ) ;
		//controlsBG.draw( ) ;
	//		timeline.draw( ) ; 

	if ( bDisableTimeout == true ) 
		IdleInputManager::Instance()->disable() ; 

}

void VideoPlayer::stop()
{
    ofLog( OF_LOG_VERBOSE , " about to stop the video on close..." ) ;
    video.close() ;
    Tweenzor::add( &alpha , alpha , 0.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ;
	ofSetFrameRate( 30 ) ; 

	if ( bDisableTimeout == true ) 
	{
		IdleInputManager::Instance()->enable() ; 
		
	}
	IdleInputManager::Instance()->input( ) ; 
}

VideoPlayer::~VideoPlayer()
{
    video.close() ;
    //timeline.clear() ;
    restart.clear() ;
}

