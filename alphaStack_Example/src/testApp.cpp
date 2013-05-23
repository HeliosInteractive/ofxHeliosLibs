#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofBackground( 0 , 0, 0 ) ;
    ofSetFrameRate( 60 ) ;
    ofSetRectMode( OF_RECTMODE_CENTER ) ;
    ofEnableAlphaBlending( ) ;

    for ( int i = 0 ; i < 12 ; i++ )
    {
        myAlphaStackItem item ;
        item.setup( ofRandom ( 100 , ofGetWidth() - 100 ) , ofRandom ( 100 , ofGetHeight() - 100 ) , 3 ) ;
        alphaStack.push_back ( item ) ;
    }
}

//--------------------------------------------------------------
void testApp::update(){

    for ( int i = 0 ; i < alphaStack.size() ; i++ )
    {
        float stackVisibility = sin ( ofGetElapsedTimef() * (( float) i / 5.0f )) * .5 + 0.5 ;
        alphaStack[i].alphaStackUpdate( stackVisibility ) ;
		alphaStack[i].update(); 
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for ( int i = 0 ; i < alphaStack.size() ; i++ )
    {
        alphaStack[i].draw ( ) ;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
