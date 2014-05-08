#include "GestureCursor.h"

GestureCursor::GestureCursor( ) 
{
	id = -1 ; 
	status = LOST ; 
	worldScale = 1.0f ; 
	smoothing = 0.5f ; 
}

GestureCursor::~GestureCursor( ) 
{
	id = -1 ; 
	status = LOST ; 
}

void GestureCursor::setup ( int _id  ) 
{
	id = _id ;
	dragLostTimer.setup( 200 , "DEPTH CURSPR DRAG TIMER" , 19 , false ) ; 
	ofAddListener( dragLostTimer.TIMER_COMPLETE, this , &GestureCursor::dragLostTimerComplete ) ; 
	//void dragLostTimerComplete ( int & args ) ; 
}

void GestureCursor::update( ) 
{
	dragLostTimer.update( ); 
}

void GestureCursor::draw3D( ) 
{
	switch ( status ) 
	{
		case LOST :
			ofSetColor( 128 , 128 , 128 , 128 ) ; 
		break; 

		case OPEN :
			ofSetColor( 0 , 255 , 0 , 128 ) ; 
		break; 

		case CLOSED :
			ofSetColor( 255 , 0 , 0 , 128 ) ; 
		break; 

		case LASSO :
			ofSetColor( 0 ,  0 , 255 , 128 ) ; 
		break;

	}

	ofPushMatrix( ) ; 
		ofTranslate( worldPosition ) ;
		ofDrawSphere( 20 ) ; 
	ofPopMatrix( ) ;
}

void GestureCursor::drawScreen( ) 
{
	ofPushStyle(); 
	switch ( status ) 
	{
		case LOST :
			ofSetColor( 128 , 128 , 128 , 128 ) ; 
		break; 

		case OPEN :
			ofSetColor( 0 , 255 , 0 , 128 ) ; 
		break; 

		case CLOSED :
			ofSetColor( 255 , 0 , 0 , 128 ) ; 
		break; 

		case LASSO :
			ofSetColor( 0 ,  0 , 255 , 128 ) ; 
		break;

	}

	ofCircle( screenPosition.x , screenPosition.y , 15 ) ; 

	ofNoFill() ; 
	ofSetColor( 255 ) ; 
	ofSetLineWidth ( 3 ) ; 
	ofCircle( screenPosition.x , screenPosition.y , 16 ) ; 

	ofPopStyle() ; 
}

void GestureCursor::parseOscMessage( ofxOscMessage * m )  
{
	/*
	stringstream ss ; 
		ss << " address " << m.getAddress() << endl << "[0] - status " << m.getArgAsInt32( 0 ) <<  endl << "[1] body id - " << m.getArgAsInt32( 1 ) << endl ;
		ss << "[2] - x " << m.getArgAsFloat( 2 ) << endl << "[3] - y " << m.getArgAsFloat( 3 ) << endl ;
		ofLogNotice( "incoming OSC MESSAGE " ) << endl << ss.str() << endl ; 
		*/

	int bodyId = m->getArgAsInt32( 0 ) ; 
	int handStatus = m->getArgAsInt32( 1 ) ;

	CURSOR_STATUS pStatus = status ; 

	switch ( handStatus ) 
	{
		case 1 : 
		status = CLOSED ; 
		break ; 

		case 2 : 
		status = OPEN ; 
		break ; 

		case 3 : 
		status = LASSO;
		break ; 

		default : 
		status = LOST ; 
		break ; 
	}

	Tweenzor::add( &normalizedScreen.x , normalizedScreen.x , m->getArgAsFloat( 2 ) , 0.0f , smoothing ) ; 
	Tweenzor::add( &normalizedScreen.y , normalizedScreen.y , m->getArgAsFloat( 3 ) , 0.0f , smoothing ) ; 


	screenPosition.x = ofLerp( ofMap( normalizedScreen.x , 0.0f , 1.0f , 0.0f , ofGetWidth() , true ) , screenPosition.x ,smoothing ) ;
	screenPosition.y = ofLerp( ofMap( normalizedScreen.y , 0.0f , 1.0f , 0.0f , ofGetHeight() , true ) , screenPosition.y , smoothing ) ;

	Tweenzor::add( &normalizedWorld.x , normalizedWorld.x , m->getArgAsFloat( 4 ) , 0.0f , smoothing ) ; // * worldScale ; 
	Tweenzor::add( &normalizedWorld.y , normalizedWorld.y , m->getArgAsFloat( 5 ) , 0.0f , smoothing ) ; //* worldScale ; 
	Tweenzor::add( &normalizedWorld.z , normalizedWorld.z , m->getArgAsFloat( 6 ) - 1.0 , 0.0f , smoothing ) ; // * worldScale ; 

	worldPosition = normalizedWorld * worldScale ; 

	//Calculate some state changes...
	if ( pStatus != LOST && status == LOST ) 
	{
		dragLostTimer.start( false , false ) ;
//		ofNotifyEvent( UP , id ) ; 
	}

	if ( pStatus == OPEN && status == CLOSED ) 
	{
		//GestureEvents
		ofNotifyEvent( GestureEvents::DOWN , id ) ; 
	}

	if ( pStatus == CLOSED && status == CLOSED ) 
	{
		ofNotifyEvent( GestureEvents::DRAG , id ) ; 
	}

	if ( pStatus == CLOSED && status != CLOSED ) 
	{
		//if ( dragLostTimer.bIsRunning == false ) 
		dragLostTimer.start( false , false ) ;
		status = CLOSED ; 
		ofNotifyEvent( GestureEvents::DRAG , id ) ;
	}

}

void GestureCursor::dragLostTimerComplete( int &args ) 
{
	ofNotifyEvent( UP , id ) ;
	status = OPEN ; 
}

