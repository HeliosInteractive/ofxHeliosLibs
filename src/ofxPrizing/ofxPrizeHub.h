#ifndef PrizeHub_H
#define PrizeHub_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxPrizeHub
{
    private :
        ~ofxPrizeHub();
        ofxPrizeHub();
    
        ofxXmlSettings folderXml ;
        string outputDirPath ;
        string outputFileName ;
    
    public:
        ofEvent<string> PRIZE_SELECTED ;

        void setup ( ) ;
        void addPrizeToBuffer ( string csvEntry , string contactInfo ) ;
        void changeOutputFolder( string folderPath = "NONE" ) ; 
    
        void exportFile() ;
        string getDirectoryPath( ) { return outputDirPath ; } 
    
		ofBuffer prizeBuffer ; 

        static ofxPrizeHub* Instance()
        {
            static ofxPrizeHub inst;
            return &inst;
        }
    
};

#endif // HELIOSTRACKER_H
