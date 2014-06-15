#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel( OF_LOG_VERBOSE ) ;
	//Always use the static - there's no private constructor so you shouldn't get any funkiness.
	ofxPrizeHub::Instance()->setup() ;
	ofxPrizeConfiguration config ; 
	config.loadFromCSV( "prize_config.csv" ) ; 
	
	//Get the prize data object from the config object
	//The bool variable determines if there is a "default" prize returned
	//otherwise it will return what is essentially an empty prize object
	ofxPrizeData pd = config.getTodaysPrizeData( true ) ; 
	//We can create a prizeGiveaway or add a prize giveaway to a PrizeGroup is there's more of a hierarchy to distributing prizes
	prize.setup( pd , ofxPrizeGiveaway::TIMED_POOL ) ; 

	ofLogNotice( "Today's prize is : " ) << pd.toString() ; 
	ofBackground( 15 ) ;

	bCheckTodaysDate = false ; 
}

//--------------------------------------------------------------
void ofApp::update(){

	//In the event that the app runs over multiple days there needs to be a check that resets everything after midnight
	if ( bCheckTodaysDate == true && ofGetSeconds() == 30 )
    {
        ofLog( OF_LOG_VERBOSE , "CHECKING THE DATE!" ) ;
		prize.checkTodaysDate() ; 
        bCheckTodaysDate = false ;
    }
    if ( ofGetSeconds() == 31 )
    {
        bCheckTodaysDate = true ;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

	stringstream ss ; 
	ss << "P - to query a prize" << endl ; 
	ss << "R - to refill prize stashes" << endl ;
	ss << prize.prizeData.toString() << endl ; 
	ofDrawBitmapStringHighlight( ss.str() , 50 , 50 ) ; 

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if ( key == 'p' || key == 'P' ) 
	{
		//This function returns a bool if you need to change the GUI / adjust logic
		//If you pass in a contact info ( email / phone / ID etc ) it will log it in the log and it can be used for clients later
		string email = "test@test.com" ;    
		prize.getPrizeResult( email ) ;
		//This may seem redundant, but if there was more than 1 prize we'd want all prizes awarded in one aggregate file
		ofxPrizeHub::Instance()->addPrizeToBuffer( prize.prizeData.prizeName , email ) ;  
	}

	if ( key == 'r' || key == 'R' ) 
	{
		ofLogNotice() << " refilling prize pool" << endl ;
		prize.refill() ; 
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
