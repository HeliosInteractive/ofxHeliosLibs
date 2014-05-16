#include "VideoTimeline.h"

VideoTimeline::VideoTimeline()
{
    //ctor
}

VideoTimeline::~VideoTimeline()
{
    //dtor
}

void VideoTimeline::setup ( string path )
{
    xmlPath= "noPath" ;
    lastIdleMarkerTime = -5.0f ;
    idleMarkerDelay = 5.0f ;
    bCheckMarkerStop = true ;
//    nextMarker  = 0 ;
    tweenVideoPosition= 0.0f ;
    bDebugTimeline = false ;
    activeDragMarkerIndex = -2 ;
    offset = ofPoint ( 0 , 0 ) ;
    video.loadMovie( path ) ;
    video.setVolume( 0.0f ) ;
    video.setLoopState( OF_LOOP_NONE ) ;
    bIsDragging = false ;
    float xPadding = 100 ;
    trackBounds = ofRectangle( -video.getWidth()/2 + xPadding , video.getHeight()/2 - 130 , video.getWidth() - ( xPadding * 2.0f ) , 100.0f ) ;
    sliderTrack.loadImage( "timeline/SliderBar.png" ) ;
    cout << "@ videoTimeline::setup : trackBounds.width : " << trackBounds.width << endl;
    //loadMarkerXml() ;

    iconPlayhead.loadImage( "timeline/SliderButton.png" ) ;
    iconPlayhead.setAnchorPercent( 0.5 , 0.5 ) ;
    iconX = 0 ;
    currentIcon = 0 ;


}

void VideoTimeline::update( )
{
    if( Tweenzor::getTween( &tweenVideoPosition ) != NULL )
    {
        video.setPosition( tweenVideoPosition ) ;
        video.update() ;
    }
    else
    {

        float pos = video.getPosition( ) ;
        float nextPos = -2 ;

        if ( lastIdleMarkerTime > 0 )
        {
            float timeRatio = ofMap( ofGetElapsedTimef() , lastIdleMarkerTime , lastIdleMarkerTime + idleMarkerDelay , 0.0f , 1.01f ) ;
          //  cout << "time ratio : " << timeRatio << endl ;
            if ( timeRatio >= 1.0f )
            {
                cout << " idle time is over !!! currentIcon "  << currentIcon << endl ;
                video.setPaused( false ) ;
                video.play( ) ;

                //if ( currentIcon < icons.size() -1 )
                if ( currentIcon < icons.size() -1 )
                    currentIcon++ ;

                if ( currentIcon == (icons.size() - 1 ) )\
                {
                    //bCheckMarkerStop
                    bCheckMarkerStop = false ;
                }
                 for ( int i = 0; i < icons.size() ; i++ )
                {
                    icons[ i ]->bActive = false ;
                }

                lastIdleMarkerTime = -2 ;
                Tweenzor::add( &iconPlayheadX , iconPlayheadX ,  icons[ currentIcon ]->x , 0.25f , 0.55F , EASE_OUT_QUAD ) ;
                video.update( ) ;

                //int _args = 213 ;
                //ofNotifyEvent( IdleCoordinator::Instance()->DELAY_IDLE , _args ) ;
                return ;
            }
        }
    //lastIdleMarkerTime
        //if ( bCheckMarkerStop == true && currentIcon != ( markers.size()-1) && pos > markers[currentIcon]->stopVideoPerc )
        if ( bCheckMarkerStop == true && lastIdleMarkerTime < 0 && pos > markers[currentIcon]->stopVideoPerc )
        {
            cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl ;
            cout << "bCheckMarkerStop" << endl ;
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl ;

            cout << "currentIcon : " << currentIcon << " curPosition : " << pos << " out position : " << markers[currentIcon ]->stopVideoPerc << endl ;

            lastIdleMarkerTime = ofGetElapsedTimef() ;

            video.setPaused( true ) ;
           // if ( nextMarker < ( markers.size() -1 ) )
           // {
               // if ( nextMarker > 0 )
               //     icons[nextMarker-1]->bActive = false ;

               //  icons[nextMarker]->bActive = true ;
               // cout << " past marker #" << nextMarker  << endl ;

                //for ( int i = 0; i < icons.size() ; i++ )
               // {
               //     icons[ i ]->bActive = false ;
               // }

                //currentIcon++ ;

                //icons [ nextMarker ]->bActive = true ;

                //nextMarker++ ;
               // cout << "next marker is now: " << nextMarker << endl ;

               // icons[nextMarker]->bActive = true ;
            //}
            //else
           // {

              //  if ( video.isPlaying() == false )
             //   {
             //       cout << "KEEP PLAYING !!! " << endl ;
             //       video.setPaused( false ) ;
             //       video.play( ) ;
              //  }
                 //video.play( ) ;
            //}


        }
    }

    video.update( ) ;
    /*
    if ( video.isPlaying() == true )
    {

        video.idleMovie() ;
    }*/

    //Tweenzor::add ( &tweenVideoPosition , video.getPosition() , newVideoPosition , 0.0f , 0.35f , EASE_OUT_QUAD ) ;


}

void VideoTimeline::videoTweenEnd( float * args )
{
    cout << "video done tweening to ! " << tweenVideoPosition << endl;
    video.setPaused( false ) ;
    video.play ( ) ;
}

/*

void drawVideo ( ) ;
        void drawTimeline( ) ;
        */
void VideoTimeline::drawVideo( )
{
    ofPushMatrix( ) ;
        ofTranslate( offset.x , offset.y ) ;
        video.draw( 0 , 0 ) ;
    ofPopMatrix( ) ;
}

void VideoTimeline::drawTimeline ( )
{
    ofPushMatrix( ) ;
        ofTranslate( offset.x , offset.y ) ;

        icons[ currentIcon ]->bActive = true ;

        ofEnableAlphaBlending( ) ;
        //ofNoFill( ) ;
       // ofSetColor( 215 , 215 , 215 , 125 )  ;
        //ofRect( trackBounds ) ;
        //ofFill( ) ;

        sliderTrack.draw( trackBounds.x , trackBounds.y + trackBounds.height / 2 + sliderTrack.height / -2 , trackBounds.width , sliderTrack.height/2 ) ;

      //  icons.clear( ) ;
        ofPushMatrix( ) ;
            ofTranslate( trackBounds.x , trackBounds.y ) ;

            float iconsOffset = 1.0f / (float)(icons.size()+1 ) ;
          //  ofTranslate( (iconsOffset/2.0f) * trackBounds.width , 125 ) ;
            ofTranslate( 35.0f  , 25 ) ;
            for ( int i = 0 ; i < icons.size() ; i++ )
            {
                ofSetColor( 255 , 255 , 255 ) ;
                icons [ i ]->draw( ) ;
            }
        ofPopMatrix() ;

        //ofSetColor( 255 , 255 , 65 , 205 ) ;
        //ofLine ( trackBounds.x , trackBounds.y + trackBounds.height/2 , trackBounds.x + trackBounds.width , trackBounds.y + trackBounds.height/2  ) ;

        float trackMapCoords = ofMap ( tweenVideoPosition , markers[0]->videoPerc , markers[ (markers.size()-1) ]->videoPerc , 0.1f, 0.9f ) ;
        float bX = trackMapCoords * trackBounds.width ;

        if ( Tweenzor::getTween( &tweenVideoPosition ) == NULL )
        {
            //bX = icons[ currentIcon ]->x ;
          //  iconPlayheadX
        }
        else
        {
           // cout << "bX : " << bX << endl ;
        }



        ofSetColor( 255 , 255 , 255 , 255 ) ;
        iconPlayhead.draw( iconPlayheadX + trackBounds.x , trackBounds.y  + 35.0f , iconPlayhead.width * .5 , iconPlayhead.height * .5f ) ;
        //ofCircle( bX , trackBounds.y + trackBounds.height/2 , 35 ) ;

        if ( bDebugTimeline == true )
        {
            ofEnableAlphaBlending( ) ;
            ofNoFill( ) ;
            ofSetColor( 215 , 215 , 215 , 125 )  ;
            ofRect( trackBounds ) ;
            ofFill( ) ;

            float playheadX = video.getPosition() * trackBounds.width ;
            for ( int m = 0 ; m < markers.size() ; m++ )
            {
                float mX = markers[m]->videoPerc * trackBounds.width ;
                ofPushMatrix( ) ;
                    ofTranslate( mX - trackBounds.x , trackBounds.y ) ;
                    markers[m]->draw( ) ;
                ofPopMatrix( ) ;
            }
            ofSetColor( 255 , 0 , 0 , 125 ) ;
            ofRect( trackBounds.x + playheadX , trackBounds.y , 15 , trackBounds.height ) ;
        }
        else
        {

        }
    ofPopMatrix() ;
}

void VideoTimeline::loadMarkerXml( string _xmlPath )
{
    xmlPath = _xmlPath ;
     cout << "loading marker XML"<< xmlPath << endl;
    ofxXmlSettings markerXml ;
    markerXml.loadFile( xmlPath ) ;

    int numMarkers = markerXml.getNumTags( "marker" ) ;
    markers.clear( ) ;

    for ( int m = 0 ; m < numMarkers ; m++ )
    {
        markerXml.pushTag("marker" , m ) ;
            float perc = markerXml.getValue( "percent" , 0.0f ) ;
            float outPerc = markerXml.getValue( "outPercent" , 0.1f ) ;
            cout << "@ marker : " << m << " in : " << perc << " <-> " << outPerc << endl; //" markers : " << m <<  " % = " << perc << endl ;
            addNewMarker( perc , outPerc ) ;
        markerXml.popTag( ) ;
    }

    cout <<" making icons! " << endl ;
    icons.clear( ) ;

    vector<string> labels ;
    labels.push_back( "PER SECOND" ) ;
    labels.push_back( "PER MINUTE" ) ;
    labels.push_back( "PER HOUR" ) ;
    labels.push_back( "PER DAY" ) ;
    labels.push_back( "PER MONTH" ) ;
    labels.push_back( "PER YEAR" ) ;

    cout << "trackBounds.width : " << trackBounds.width << endl ;
    for ( int i = 0 ; i < numMarkers ; i++ )
    {
        TimelineIcon * tIcon = new TimelineIcon( ) ;
        float percent =  ((float)(i)) / (float)(numMarkers-1) ;
        cout << "percent : " << percent << endl;

        float mappedPercent = ofMap( percent , 0.0f , 1.0f , 0.02f , 0.98f ) ;
        cout << "percent :  " << percent << " vs. mappedPercent : " << mappedPercent << endl ;
        tIcon->setup( mappedPercent * trackBounds.width , 0.0f , "timeline/SliderButton.png" , labels[i] ) ;
        icons.push_back( tIcon ) ;
    }

     iconPlayheadX = icons[ currentIcon ]->x ;
    normalizedinputUp( 0.01 , 0.787667 ) ;
}

void VideoTimeline::saveMarkerXml( )
{
    cout << "saving marker XML"<< endl;
    ofxXmlSettings markerXml ;

    for ( int m = 0 ; m < markers.size() ; m++ )
    {
        markerXml.addTag( "marker" ) ;
        markerXml.pushTag("marker" , m ) ;
            markerXml.setValue( "percent" , markers[m]->videoPerc ) ;
        markerXml.popTag( ) ;
    }
    markerXml.saveFile( xmlPath ) ;
}

void VideoTimeline::addNewMarker( float videoPosition , float outPosition )
{
    if ( videoPosition <  0 )
    {
        videoPosition = video.getPosition( )  ;
    }
    TimelineMarker * marker = new TimelineMarker( ) ;
    marker->setup( markers.size() , videoPosition , outPosition ) ;
    markers.push_back( marker ) ;
}


void VideoTimeline::play( )
{
    video.play() ;
    video.setPaused( false ) ;
}

void VideoTimeline::snap ( )
{
    video.stop() ;
}

void  VideoTimeline::togglePlay()
{


    bool bPauseStatus = video.isPaused() ;
    if ( bPauseStatus == true )
    {
        cout << "was paused" << endl;
    }
    else
    {
        cout << "was playing " << endl ;
    }
    bPauseStatus = !bPauseStatus ;
    video.setPaused( bPauseStatus ) ;
}

bool VideoTimeline::hitTest( float _x , float _y )
{
    float x = _x - offset.x ;
    float y = _y - offset.y ;

    if ( ( x >= trackBounds.x && x <= ( trackBounds.x + trackBounds.width ) ) &&
          ( y >= trackBounds.y && y <= ( trackBounds.y + trackBounds.height )  ) )
    {
            //cout<< "in bounds!" << endl ;
            return true ;
    }
    else
    {
        return false ;
    }
}


int VideoTimeline::hitMarkerTest( float _x , float _y )
{
    int hitMarker = -1 ;
    float x = _x - offset.x ;
    float y = _y - offset.y ;

    for ( int m = 0 ; m < markers.size() ; m++ )
    {
        float mX = markers[m]->videoPerc * trackBounds.width ;

        if ( ( x >= trackBounds.x + mX && x <= ( trackBounds.x + mX + markers[m]->bounds.width ) ) &&
             ( y >= trackBounds.y && y <= ( trackBounds.y +  markers[m]->bounds.height ) ) )
        {
                //cout<< "in bounds!" << endl ;
                return m ;
        }
    }
    return -1 ;

}

void VideoTimeline::inputDown ( float x , float y )
{

    if ( bDebugTimeline == true )
    {
        int index =  hitMarkerTest( x , y ) ;

        if ( index >= 0 )
        {
            cout << "returned markerHitTest Index : " << index << endl ;
            activeDragMarkerIndex = index ;
        }
    }
    else
    {

    }

    inputDrag( x , y ) ;
 }

void VideoTimeline::inputDrag ( float x , float y )
{
    if ( bDebugTimeline == true )
    {
        bool bDragHitTest = hitTest ( x , y ) ;

        float newVideoPosition = ofMap ( x - offset.x , trackBounds.x , trackBounds.x + trackBounds.width , 0.0f , 1.0f ) ;
        if ( activeDragMarkerIndex >= 0 )
        {
            markers[ activeDragMarkerIndex ]->videoPerc = newVideoPosition ;
        }
        if ( bDragHitTest == true )
        {
            float newVideoPosition = ofMap ( x - offset.x , trackBounds.x , trackBounds.x + trackBounds.width , 0.0f , 1.0f ) ;
            video.setPosition( newVideoPosition ) ;
            video.update( ) ;
            video.setPaused( true ) ;
        }
        else
        {

        }
    }
}

void VideoTimeline::removeMarkerAt( int markerIndex )
{
    if( markerIndex < 0 || markerIndex >= markers.size() )
    {
        cout << "invalid remove index " << endl ;
        return ;
    }
    int count = 0 ;
     vector<TimelineMarker*>::iterator m ;
    for ( m = markers.begin() ; m < markers.end() ; m++ )
    {
        if ( count == markerIndex )
        {
            markers.erase( m ) ;
        }
        count++ ;
    }
    //cout << " " << *it;

  cout << endl;
}

void VideoTimeline::normalizedinputUp ( float x , float y )
{
    bCheckMarkerStop = true ;
     float positionDiff = video.getPosition( ) ;
     //cout << "normalized : " << x << " between 0.0f , 1.0f "
            float newVideoPosition = ofMap ( x , 0.1f , 0.9f , markers[0]->videoPerc , markers[ (markers.size()-1) ]->videoPerc ) ;
            cout << "new video position :: " << newVideoPosition << endl ;
            positionDiff = abs( positionDiff - newVideoPosition ) ;

            for ( int i = 0 ; i < icons.size() ; i++ )
            {
                icons[ i ]->bActive = false;
            }

            float closestDistance = 1000.0f ;
            int closestIndex = -1 ;
            for ( int m = 0 ; m < markers.size() ; m++ )
            {

                float perc = markers[m]->videoPerc ;
                float dist = abs( newVideoPosition - perc ) ;
                if ( dist < closestDistance )
                {
                    closestDistance = dist ;
                    closestIndex = m ;
                }

                if ( closestIndex == markers.size() -1 )
                {
                    bCheckMarkerStop = false ;
                }
            }

            if ( closestIndex >= 0 )
            {
                currentIcon = closestIndex ;
                newVideoPosition = markers[ closestIndex ]->videoPerc ;
                cout << "snapping to : " << closestIndex << " index " << endl ;
             //   if ( closestIndex < (markers.size()-1) )
             //       nextMarker = closestIndex+1 ;
            }
            else
            {
                cout << "no index found ! " << endl;
            }

            // If it's close only have it go .25 , otherwise...
            float positionTweenTime = ofMap(  positionDiff , 0.0f , 1.0f , 0.35f , 1.25f ) ;
            Tweenzor::add ( &tweenVideoPosition , video.getPosition() , newVideoPosition , 0.0f , positionTweenTime , EASE_OUT_QUAD ) ;
            Tweenzor::addCompleteListener( Tweenzor::getTween( &tweenVideoPosition ) , this , &VideoTimeline::videoTweenEnd ) ;

            Tweenzor::add( &iconPlayheadX , iconPlayheadX ,  icons[ currentIcon ]->x , 0.0f , positionTweenTime , EASE_OUT_QUAD ) ;

            lastIdleMarkerTime= -2 ;

}
void VideoTimeline::inputUp ( float x , float y )
{
    activeDragMarkerIndex = -2 ;
    if ( bDebugTimeline == true )
    {

    }
    else
    {
        bool bHitTest = hitTest( x , y ) ;
        if ( bHitTest == true )
        {
            bCheckMarkerStop = true ;
            float positionDiff = video.getPosition( ) ;
            float newVideoPosition = ofMap ( x - offset.x , trackBounds.x , trackBounds.x + trackBounds.width , markers[0]->videoPerc , markers[ (markers.size()-1) ]->videoPerc ) ;
            positionDiff = abs( positionDiff - newVideoPosition ) ;

            for ( int i = 0 ; i < icons.size() ; i++ )
            {
                icons[ i ]->bActive = false;
            }

            float closestDistance = 1000.0f ;
            int closestIndex = -1 ;
            for ( int m = 0 ; m < markers.size() ; m++ )
            {

                float perc = markers[m]->videoPerc ;
                float dist = abs( newVideoPosition - perc ) ;
                if ( dist < closestDistance )
                {
                    closestDistance = dist ;
                    closestIndex = m ;
                }

                if ( closestIndex == markers.size() -1 )
                {
                    bCheckMarkerStop = false ;
                }
            }

            if ( closestIndex >= 0 )
            {
                currentIcon = closestIndex ;
                newVideoPosition = markers[ closestIndex ]->videoPerc ;
                cout << "snapping to : " << closestIndex << " index " << endl ;
               // if ( closestIndex < (markers.size()-1) )
               //     nextMarker = closestIndex+1 ;
            }
            else
            {
                cout << "no index found ! " << endl;
            }

            // If it's close only have it go .25 , otherwise...
            float positionTweenTime = ofMap(  positionDiff , 0.0f , 1.0f , 0.00f , 0.85f ) ;
            Tweenzor::add ( &tweenVideoPosition , video.getPosition() , newVideoPosition , 0.0f , positionTweenTime  ) ;
            Tweenzor::addCompleteListener( Tweenzor::getTween( &tweenVideoPosition ) , this , &VideoTimeline::videoTweenEnd ) ;

            lastIdleMarkerTime= -2 ;
            //videoTweenEnd
         }

    }
}
