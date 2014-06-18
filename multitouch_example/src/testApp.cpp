#include "testApp.h"
#include "MultiTouchActions.h"
//--------------------------------------------------------------
void testApp::setup()
{
	Tweenzor::init(); 

    multiTouchObject.setup( ofGetWidth()/2 , ofGetHeight()/2 , 400.0f , 0.9725f ) ;
    ofSetFrameRate( 60 ) ;
}

//--------------------------------------------------------------
void testApp::update(){

	Tweenzor::update( ofGetElapsedTimeMillis() ) ;
    multiTouchObject.update( ) ;
}

//--------------------------------------------------------------
void testApp::draw(){

    multiTouchObject.draw( ) ;

	ofDrawBitmapStringHighlight(  multiTouchObject.getDebugOutput() , 15 , 15 ) ; 

	map<int,ofPoint>::iterator iter;
	for ( iter=touches.begin(); iter != touches.end(); ++iter )
	{
		ofCircle(iter->second, 30);
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

// win7 events
void testApp::touchDown(ofTouchEventArgs &touch)
{
	multiTouchObject.multiTouch.touchDown( touch.x /ofGetWidth() , touch.y / ofGetHeight() , touch.id ) ;
}

void testApp::touchMoved(ofTouchEventArgs &touch ) 
{
	multiTouchObject.multiTouch.touchDrag( touch.x / ofGetWidth()  , touch.y / ofGetHeight() , touch.id ) ;
}

void testApp::touchUp(ofTouchEventArgs &touch)
{
	multiTouchObject.multiTouch.touchUp( touch.x / ofGetWidth()  , touch.y / ofGetHeight() , touch.id ) ;
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	/*
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onDrag( ofPoint ( x/w , y/h )  ) ;
    mouseDragReflect.onDrag( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h));

    ofPoint m = ofPoint( x , y ) ;
    ofPoint myObjP = ofPoint ( mtObject.x * w , mtObject.y * h ) ;
    //OVERLY SIMPLE hit detection. We check how far our point is from the center ro see if we are hitting
    float dist = myObjP.distance( myObjP ) ;
    if ( dist < ( mtObject.area.width / 2 ) )
    {
        mtObject.multiTouch.touchDrag( mouseDrag ) ;
        mtObject.multiTouch.touchDrag( mouseDragReflect ) ;
    }*/
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	/*
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onDown( ofPoint ( x/w , y/h )) ;
    mouseDragReflect.onDown( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h));

    ofPoint m = ofPoint( x , y ) ;
    ofPoint myObjP = ofPoint ( mtObject.x * w , mtObject.y * h ) ;

    //OVERLY SIMPLE hit detection. We check how far our point is from the center To see if we are hitting
    float dist = myObjP.distance( myObjP ) ;
    if ( dist < ( mtObject.area.width / 2 ) )
    {
        cout << "pressed within range!" << endl ;
        mtObject.multiTouch.touchDown( mouseDrag ) ;
        mtObject.multiTouch.touchDown( mouseDragReflect ) ;
    }*/
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	/*
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onUp( ofPoint ( x/w , y/h ) ) ;
    mouseDragReflect.onUp( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h ));

    //On up we always want to remove points ?
    mtObject.multiTouch.touchRelease( mouseDrag ) ;
    mtObject.multiTouch.touchRelease( mouseDragReflect ) ;
	*/
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
