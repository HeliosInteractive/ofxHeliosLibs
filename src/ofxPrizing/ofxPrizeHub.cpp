#include "ofxPrizeHub.h"

ofxPrizeHub::ofxPrizeHub()
{
    //ctor
}

ofxPrizeHub::~ofxPrizeHub()
{
    //dtor
}

void ofxPrizeHub::setup ( )
{
    outputDirPath = "" ;
    stringstream dateFileName ;
    dateFileName << ofGetYear() << "_" << ofGetMonth() << "_" << ofGetDay() << "_prizes.csv" ;
    outputFileName = dateFileName.str() ;
    
    bool bResult = folderXml.loadFile("prize_folder_dir.xml") ;    
    if ( bResult == true )
    {
        changeOutputFolder ( folderXml.getValue("folderPath", ofToDataPath("" ) ) )  ; 
    }
    else
    {
        changeOutputFolder() ;
    }
    
    ofLogError( "full Path : " ) << outputDirPath << " + " << outputFileName << endl ;

    if ( ofFile::doesFileExist( outputDirPath +"/" + outputFileName ) )
    {
        ofLogError( "FILE EXISTS, loading it's data" ) ;
        prizeBuffer = ofBufferFromFile( outputDirPath +"/" + outputFileName , false  ) ;
        //exportFile();
    }
    else
    {
        ofLogError( " CSV DOES NOT EXIST. CREATING FROM SCRATCH" ) ;
        string titleData = "PRIZE NAME,CONTACT INFO,HOUR,MINUTE,SECONDS,\n"  ;
        prizeBuffer.set( titleData.c_str() , titleData.size() ) ;
        exportFile() ;
    }
}

void ofxPrizeHub::exportFile( )
{
    ofBufferToFile( outputDirPath + "/" + outputFileName , prizeBuffer ) ;
}

void ofxPrizeHub::addPrizeToBuffer ( string prizeName , string contactInfo )
{
    stringstream csvRow ;
    csvRow << prizeName << "," << contactInfo << "," << ofGetHours() << "," << ofGetMinutes() << "," << ofGetSeconds() << "," << endl ;
    
    prizeBuffer.append( csvRow.str() ) ;
    exportFile() ; 
}

void ofxPrizeHub::changeOutputFolder( string folderPath  )
{
    string path = ofToDataPath("" ) ; 
    if ( folderPath != "NONE" )
    {
        outputDirPath = folderPath ;
    }
    else
    {
        ofLogError( "ofxPrizeHub::changeOutputFolder NO FOLDER, choosing now ") ;
        ofFileDialogResult result = ofSystemLoadDialog("CHOOSE OUTPUT CSV FOLDER" , true ) ;
        if ( result.bSuccess == true )
        {
            outputDirPath = result.getPath() ; 
        }
        else
        {
            ofLogError( "DID NOT CHOOSE A FOLDER , DEFAULTING TO DATA PATH " ) ;
            outputDirPath = ofToDataPath("") ;
            return ;
        }
    }
    
    folderXml.setValue("folderPath", outputDirPath ) ;
    folderXml.saveFile("prize_folder_dir.xml") ;
    
    ofSetDataPathRoot( path ) ; 
}
