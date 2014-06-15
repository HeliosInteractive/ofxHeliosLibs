//
//  ofxPrizeGiveaway.h
//  emptyExample
//
//  Created by Ben McChesney on 8/18/12.
//
//


#include "ofxPrizeGiveaway.h"



void ofxPrizeGiveaway::setup ( ofxPrizeData pd , DistributionMethod _method )
{
	prizeData = pd ; 
    awardMethod = _method ; 
    ofDirectory dir ;
	dir.createDirectory( ofToDataPath( "prizes/"+pd.prizeName ) , true , true ) ;
    originalPath = xmlPath ;
    checkTodaysDate() ;
}

void ofxPrizeGiveaway::checkTodaysDate( )
{
    ofxCalendar date ;
    date.setToCurrent() ;

    ofxXmlSettings xmlTest ;
    string todayPath = "prizes/" + prizeData.prizeName + "/" + prizeData.prizeName + "_" + date.getFilePath( ) + ".xml" ;
    bool bResult = xmlTest.loadFile( todayPath ) ;
    ofLog( OF_LOG_VERBOSE , "attempting to load : " + todayPath + " bResult : " + ofToString( bResult )) ;
    xmlPath = todayPath ;
    if ( bResult )
    {
        ofLog( OF_LOG_VERBOSE , todayPath + " DID LOAD!"  ) ;
        prizeXml.loadFile( todayPath ) ;
        reset( ) ;
        ofLog( OF_LOG_VERBOSE , "stashed : " + ofToString ( prizeData.stashedPrizes ) + " , awarded:" + ofToString( prizeData.awardedPrizes ) + ", total prizes:" + ofToString( prizeData.totalPrizes ) ) ;
    }
    else
    {
        ofLog( OF_LOG_VERBOSE , todayPath + " did not load! We need to create a new one" ) ;
		prizeData.awardedPrizes = 0 ; 
		prizeData.stashedPrizes = prizeData.totalPrizes ; 
		writeToXml() ; 
        reset() ; 
    }
    
}

void ofxPrizeGiveaway::refill()
{
    
    prizeXml.clear() ;
    reset() ;
	prizeData.awardedPrizes = 0 ;
	prizeData.stashedPrizes = prizeData.totalPrizes ; 
    writeToXml() ; 
}

void ofxPrizeGiveaway::reset ( )
{
    bool bTagResult = prizeXml.pushTag( "prize" ) ;
	if ( bTagResult == false ) 
		prizeXml.addTag( "prize" ) ; 

    prizeData.awardedPrizes = prizeXml.getValue( "numAwarded" , 0 ) ;
	prizeData.stashedPrizes = prizeXml.getValue( "numStashed" , 0 ) ;

    prizeXml.popTag( ) ;


    ofxCalendar current ; 
	current.setToCurrent() ;
    switch ( awardMethod )
    {
        case RANDOM :
            ofLogVerbose( "RANDOM" ) ;
            break ;
            
        case TIMED :
            ofLogVerbose( "TIMED" ) ;
			ofLog( OF_LOG_VERBOSE , "hours of contest are : " + prizeData.startTime.getClockString() + "<->" + prizeData.endTime.getClockString() ) ;
            ofLog( OF_LOG_VERBOSE , "in minutes that is :" + ofToString( prizeData.startTime.toMinutes() ) + "<->" + ofToString( prizeData.endTime.toMinutes() ) ) ;
            ofLog( OF_LOG_VERBOSE , "give away a prize every : " + ofToString(prizeMinuteSpacing) + " totalMinutes: " + ofToString(totalMinutes) ) ;
            ofLog( OF_LOG_VERBOSE , "it is Currently : " + ofToString(getCurrentMinutes()) ) ;

            break ;
            
        case AVAILABLE :
            ofLogVerbose( "AVAILABLE" ) ;
            break ;
            
        case TIMED_POOL :
            
			totalMinutes = ( prizeData.endTime.toMinutes() - prizeData.startTime.toMinutes() ) ;
			prizeMinuteSpacing =  totalMinutes / ( prizeData.totalPrizes + 1 ) ;
			//prizeData.stashedPrizes = (( current.toMinutes() - prizeData.startTime.toMinutes() ) / prizeMinuteSpacing ) ;
			ofLogVerbose( "TIMED_POOL" ) ;
            ofLog( OF_LOG_VERBOSE , "hours of contest are : " + prizeData.startTime.getClockString() + "<->" + prizeData.endTime.getClockString() ) ;
            ofLog( OF_LOG_VERBOSE , "in minutes that is :" + ofToString( prizeData.startTime.toMinutes() ) + "<->" + ofToString( prizeData.endTime.toMinutes() ) ) ;
            ofLog( OF_LOG_VERBOSE , "give away a prize every : " + ofToString(prizeMinuteSpacing) + " totalMinutes: " + ofToString(totalMinutes) ) ;
            ofLog( OF_LOG_VERBOSE , "it is Currently : " + ofToString(getCurrentMinutes()) ) ;
            break ;
            
        default :
            break ;
    }

    
	
    ofLog( OF_LOG_NOTICE , "Giving away " + ofToString(prizeData.totalPrizes) + " of : " + prizeData.prizeName ) ;
    ofLog( OF_LOG_NOTICE , "prizes already awarded :" + ofToString(prizeData.awardedPrizes) + " , prizes stashed : " + ofToString(prizeData.stashedPrizes) ) ;

 
    int numWinnerTags = prizeXml.getNumTags("winner") ;
    ofLog( OF_LOG_VERBOSE , "total number of winner tags " + ofToString( numWinnerTags ) ) ;
    if ( numWinnerTags < prizeData.totalPrizes )
    {
        for ( int i = 0 ; i < ( prizeData.totalPrizes - numWinnerTags ) ; i++)
        {
            int index = prizeXml.addTag( "winner" ) ;
            prizeXml.pushTag( "winner" , index ) ;
            prizeXml.setValue( "AWARDED" , 0 ) ;
            prizeXml.popTag( ) ;
        }
    }
    writeToXml( ) ;
}

bool ofxPrizeGiveaway::getPrizeResult( string userContactInfo )
{
    //ofLogVerbose( "ofxPrizeGiveaway::getPrizeResult" ) << " ( " << name << " ) " <<  "stashed : " + ofToString ( prizeData.stashedPrizes ) + " , awarded:" + ofToString( prizeData.awardedPrizes ) + ", total prizes:" + ofToString( prizeData.totalPrizes )  ;

    if ( prizeData.awardedPrizes == prizeData.totalPrizes ) 
	{
		ofLogError() << "all " << prizeData.totalPrizes << " '" << prizeData.prizeName << "' have already been given away for today." ; 
		return false; 
	}

    bool bPrizeResult = false ;
    switch ( awardMethod )
    {
        case RANDOM :
            ofLogVerbose( "RANDOM" ) ;
            bPrizeResult = getPrizeRandom() ;
            break ;
            
        case TIMED :
            ofLogVerbose( "TIMED" ) ;
            bPrizeResult = getPrizeTimed() ;
            break ;
            
        case AVAILABLE :
            ofLogVerbose( "AVAILABLE" ) ;
            bPrizeResult = getPrizeAvailable() ;
            break ;
            
        case TIMED_POOL :
            ofLogVerbose( "TIMED_POOL" ) ;
            bPrizeResult = getPrizeTimedPool() ; 
            break ;
            
        default :
            break ;
    }
    
	
    
    if ( bPrizeResult == true )
    {
        prizeData.awardedPrizes++ ;
        prizeData.stashedPrizes-- ;
        
        prizeXml.pushTag("winner" , prizeData.awardedPrizes ) ;
        prizeXml.setValue( "AWARDED" , 1 ) ;
        prizeXml.setValue( "HOUR" , ofGetHours() ) ;
        prizeXml.setValue( "MINUTES" , ofGetMinutes() ) ;
        prizeXml.setValue( "SECONDS" , ofGetSeconds() ) ;
		prizeXml.setValue( "CONTACT_INFO" , userContactInfo ) ; 
        prizeXml.popTag( ) ;
        writeToXml( ) ;
        return true ; 
    }
    else
    {
        ofLogError(  "No prizes are available at this time." ) ; 
        return false ;
    }
}

bool ofxPrizeGiveaway::getPrizeRandom( )
{
    ofLogVerbose( "PRIZE RANDOM NOT IMPLEMENTED" ) ;
    return false ;
}

bool ofxPrizeGiveaway::getPrizeTimed( )
{
    ofLogVerbose( "PRIZE TIMED NOT IMPLEMENTED" ) ;
    return false ;
}

bool ofxPrizeGiveaway::getPrizeAvailable( )
{
    ofLogVerbose( "getPrizeAvailable()" ) << prizeData.prizeName << " # stashed : " << prizeData.stashedPrizes << " of total: " << prizeData.totalPrizes << endl ;
    if ( prizeData.stashedPrizes > 0 )
    {
        return true ;
    }
    else
    {
        return false ;
    }
}

bool ofxPrizeGiveaway::getPrizeTimedPool( )
{
    
    if ( prizeData.stashedPrizes > 0 && prizeData.awardedPrizes < prizeData.totalPrizes )
    {
        //Get our current minutes for math and calculations
        int curMinutes = getCurrentMinutes() ;
		if ( curMinutes > prizeData.endTime.toMinutes() )
        {
            //Not limiting this will cause more prizes to be dealt out
            ofLogError() << "PAST XML DEFINED MAX TIME" ;
			curMinutes = prizeData.endTime.toMinutes() ;
        }
        
        //We want to give away prizes a little before the very end so sometimes the math come sback a little off
        int prizesThatShouldBeUnlocked = (( curMinutes - prizeData.startTime.toMinutes() ) / prizeMinuteSpacing ) ;
        //Make sure we never give away more than we are suppose too
        if ( prizesThatShouldBeUnlocked >= prizeData.totalPrizes )
        {
            prizesThatShouldBeUnlocked = prizeData.totalPrizes ;
        }
        if ( prizeData.awardedPrizes < prizesThatShouldBeUnlocked )
        {
            int currentIndex = prizeData.awardedPrizes ;
            ofLogVerbose( "prizes awarded!" )  << prizeData.awardedPrizes << " : should be awarded : " << prizesThatShouldBeUnlocked << endl ;
            int prizeDiff = prizesThatShouldBeUnlocked - prizeData.awardedPrizes ;
            ofLogError(  "we can give away : " ) << prizeDiff << " more prizes !" << endl ;
            
            
            
            return true ;
        }
        ofLogError( "no prizes are free for the taking" ) ;
        return false ;
    }

}

int ofxPrizeGiveaway::getCurrentMinutes( )
{
    int minutes = ofGetHours() * 60 + ofGetMinutes() ;
    return minutes ;
}

void ofxPrizeGiveaway::writeToXml( )
{
    // Date date ;
    // date.setToCurrentTime( ) ;
    prizeXml.pushTag( "prize" ) ;
		prizeXml.setValue( "name", prizeData.prizeName ) ; 
        prizeXml.setValue( "numAwarded" , prizeData.awardedPrizes ) ;
        prizeXml.setValue( "numStashed" , prizeData.stashedPrizes ) ;
    prizeXml.popTag( ) ;
    prizeXml.saveFile ( xmlPath ) ;
}

