#include "VideoBank.h"


void VideoBank::setup ( float _x , float _y , float videoDelay , bool _bLoop ) 
{
	x = _x ; 
	y = _y ; 
	bLoop = _bLoop ; 
	videoDelayTimer.setup( videoDelay ) ;
	ofAddListener( videoDelayTimer.TIMER_COMPLETE , this , &VideoBank::videoDelayTimerComplete ) ; 

}

void VideoBank::videoDelayTimerComplete( int & args )
{
	if ( bLoop == true ) 
		playRandomVideo( ) ; 
}
 
void VideoBank::loadFolder( string folderPath , float _volume ) 
{
	x = 0 ; 
	y = 0 ; 
	volume = _volume ; 

	ofDirectory dir ; 
	dir.allowExt( "mov" ) ; 
	dir.allowExt( "mp4" ) ; 

	dir = ofDirectory( folderPath ) ; 
	dir.listDir( ) ; 

	if ( dir.size() < 1 ) 
	{
		ofLogError( "VideoBank::loadFolder" ) << folderPath << " could not be loaded ! " << endl ; 
	}
	else
	{
		for ( int i = 0 ; i < dir.size() ; i++ ) 
		{
			addVideo( dir.getPath( i ) , volume ) ; 
		}
	}


}

void VideoBank::addVideo( string videoPath , float _volume ) 
{
	int index = videos.size() ; 
	videos.push_back ( new ofVideoPlayer() ) ; 
	bool bLoaded = videos[ index ]->loadMovie( videoPath ) ; 
	ofLogNotice( " VideoBank::addVideo " ) << videoPath << " RESULT : " << bLoaded << endl ; 
	videos[ index ]->setVolume( _volume ) ; 
	videos[ index ]->setLoopState( OF_LOOP_NONE ) ; 
}

void VideoBank::update( ) 
{
	if ( currentVideo >= 0 ) 
	{
		videos[ currentVideo ]->update( ) ;
		
		
		if ( videos[ currentVideo ]->getIsMovieDone() == true && videoDelayTimer.bIsRunning == false )
		{
			//cout << " STARTING TIMER @"  << ofGetElapsedTimeMillis() << " when delay is " << videoDelayTimer.delayMillis <<  endl ; 
			videoDelayTimer.start( false , true ) ; 
		}
	}
	videoDelayTimer.update( ) ;
}

void VideoBank::draw( ) 
{
	ofPushMatrix( ) ; 
	//	ofTranslate( x , y , 0 ) ; 
		if ( currentVideo >= 0 ) 
		{
			//cout << "drawing ! !" << currentVideo << endl ;
			videos[ currentVideo ]->draw( 0 ,0 ) ; 
		}
	ofPopMatrix( ) ; 
}

void VideoBank::playRandomVideo( ) 
{
	if ( videos.size() == 0 ) 
	{
		ofLogError( "VideoBank::playRandomVideo()" ) << " is empty ! Not loading any sounds" << endl ; 
		return ; 
	}

    //We wouldn't want to play the same random sound 2x in a row
    int randomIndex = lastRandomIndex ;
	if ( videos.size() > 1 ) 
	{
			while ( randomIndex == lastRandomIndex )
			{
				randomIndex = ofWrap( (int)floor( ofRandom( 0 , videos.size() ) ) , 0 , videos.size() ) ;
			}
	}

	currentVideo = randomIndex ; 
    lastRandomIndex = randomIndex ;
    ofLogVerbose( "VideoBank::playRandomVideo" )  << " random index was : " << randomIndex << " of : " << (videos.size()-1) << endl ;
	//videos[ currentVideo ]->setVolume( volume ) ; 
	videos[ currentVideo ]->setFrame( 0 ) ; 
    videos[ currentVideo ]->play() ;
	videos[ currentVideo ]->update( ) ; 
}

void VideoBank::playVideoAt( int index ) 
{
	currentVideo = index ; 
    lastRandomIndex = index ;
	//videos[ currentVideo ]->setVolume( volume ) ; 
	videos[ currentVideo ]->setFrame( 0 ) ; 
    videos[ currentVideo ]->play() ;
	videos[ currentVideo ]->update( ) ; 
}


void VideoBank::stop ( ) 
{
	for ( int i = 0 ; i < videos.size() ; i++ ) 
	{
		videos[ i ]->stop( ); 
	}
}
	
void VideoBank::reset( )
{
	for ( int i = 0 ; i < videos.size() ; i++ ) 
	{
		// This seems like overkill but it will prevent the white texture showing up at the beginning 
		//  and is overall better for seamlessness
		videos[ i ]->stop( ); 
		videos[ i ]->setFrame( 0 ) ; 
		videos[ i ]->play( ) ; 
		videos[ i ]->update( ) ; 
		videos[ i ]->stop( ) ; 
	}
}