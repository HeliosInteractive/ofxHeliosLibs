//
//  VideoPlayer.h
//  emptyExample
//
//  Created by Ben McChesney on 3/28/13.
//
//

#ifndef __emptyExample__VideoPlayer__
#define __emptyExample__VideoPlayer__

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
#include "ofxSimpleImageButton.h"
#include "Tweenzor.h"

class VideoPlayer : public ofxAlphaStackItem , public ofThread
{
    public :
    void setup ( ofRectangle bounds ) ;
    void update ( ) ;
    void draw ( ) ;
    void playMovie( string path ) ; 
    void stop( ) ;
	

    ofVideoPlayer video ;
    string path ;
    ofRectangle contentRect ;

    ofImage timeline ;
    ofImage restart ;

    ofxSimpleImageButton playButton ;
    ofxSimpleImageButton pauseButton ;
    ofxSimpleImageButton resetButton ;
    ofxSimpleImageButton closeButton ;

	ofRectangle seekBarArea ;  
	bool bIsDragScrubbing ; 

    void inputDown( float x , float y ) ;
	void inputDrag( float x , float y ) ;
	void inputRelease( float x , float y ) ;
    void stopCompleteHandler( float * args ) ;

    ~VideoPlayer();

	void threadedFunction ( ) ; 
	float dragPosition ; 

	ofTexture videoTexture ; 
	bool bMouseDragged ; 
	float scrubPosition ; 

};
#endif /* defined(__emptyExample__VideoPlayer__) */
