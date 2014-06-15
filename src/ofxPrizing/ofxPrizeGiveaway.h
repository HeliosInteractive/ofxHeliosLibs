//
//  PrizeGiveaway.h
//  emptyExample
//
//  Created by Ben McChesney on 8/18/12.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxCalendar.h"
#include "ofxPrizeData.h"

class ofxPrizeGiveaway
{
    public :
    
        enum DistributionMethod
        {
            RANDOM, TIMED , AVAILABLE , TIMED_POOL
        };
    

        ofxPrizeGiveaway( ) { }

        void setup ( ofxPrizeData prizeData , DistributionMethod _method = AVAILABLE ) ;
        void checkTodaysDate( ) ;
        void reset ( ) ;
        
        int getCurrentMinutes( ) ;
        void writeToXml( ) ;
    
		bool getPrizeResult( string userContactInfo ="no email"  ) ;
		void refill() ; 

		
        string originalPath ;
		DistributionMethod awardMethod ;
        ofxXmlSettings prizeXml ;
		ofxPrizeData prizeData ; 

        string xmlPath ;

        int prizeMinuteSpacing ;
        int totalMinutes ;

protected :
        
        bool getPrizeRandom( ) ;
        bool getPrizeTimed( ) ;
        bool getPrizeAvailable( ) ;
        bool getPrizeTimedPool( ) ; 
        
};
