#include "testApp.h"
#include "MultiTouchActions.h"
//--------------------------------------------------------------
void testApp::setup()
{
    multiTouchObject.setup( ofGetWidth()/2 , ofGetHeight()/2 , 400.0f , 0.9725f ) ;
    ofSetFrameRate( 60 ) ;

#ifdef NO_MULTITOUCH
	mouseDrag.setup ( 0  , 0 , 0 ) ;
    mouseDragReflect.setup( 1 , 0 , 0 ) ;
#endif 
}

//--------------------------------------------------------------
void testApp::update(){

#ifdef NO_MULTITOUCH
	mouseDrag.update( ) ;
    mouseDragReflect.update( ) ;
#endif 

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

	mouseDrag.draw ( ) ;
    mouseDragReflect.draw( ) ;

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
	
#ifdef NO_MULTITOUCH
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onDrag( ofPoint ( x/w , y/h )  ) ;
    mouseDragReflect.onDrag( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h));

    ofPoint m = ofPoint( x , y ) ;
    ofPoint myObjP = ofPoint ( multiTouchObject.x * w , multiTouchObject.y * h ) ;
    //OVERLY SIMPLE hit detection. We check how far our point is from the center ro see if we are hitting
    float dist = myObjP.distance( myObjP ) ;
    if ( dist < ( multiTouchObject.area.width / 2 ) )
    {
        multiTouchObject.multiTouch.touchDrag( mouseDrag ) ;
        multiTouchObject.multiTouch.touchDrag( mouseDragReflect ) ;
    }
#endif
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

#ifdef NO_MULTITOUCH
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onDown( ofPoint ( x/w , y/h )) ;
    mouseDragReflect.onDown( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h));

    ofPoint m = ofPoint( x , y ) ;
    ofPoint myObjP = ofPoint ( multiTouchObject.x * w , multiTouchObject.y * h ) ;

    //OVERLY SIMPLE hit detection. We check how far our point is from the center To see if we are hitting
    float dist = myObjP.distance( myObjP ) ;
    if ( dist < ( multiTouchObject.area.width / 2 ) )
    {
        cout << "pressed within range!" << endl ;
        multiTouchObject.multiTouch.touchDown( mouseDrag ) ;
        multiTouchObject.multiTouch.touchDown( mouseDragReflect ) ;
    }
#endif
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

#ifdef NO_MULTITOUCH
    float w = (float)ofGetWidth() ;
    float h = (float)ofGetHeight() ;
    mouseDrag.onUp( ofPoint ( x/w , y/h ) ) ;
    mouseDragReflect.onUp( ofPoint ( (ofGetWidth() - x)/w , (ofGetHeight() - y )/h ));

    //On up we always want to remove points ?
    multiTouchObject.multiTouch.touchUp( mouseDrag ) ;
    multiTouchObject.multiTouch.touchUp( mouseDragReflect ) ;
#endif 
	
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
