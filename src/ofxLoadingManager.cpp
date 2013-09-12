#include "ofxLoadingManager.h"

/*
	void setup ( string localSavePath ) ; 
		void update() ; 
		void draw() ; 

		void loadURL( string _url ) ;
		void urlResponse( ofHttpResponse & response ) ; 
		bool checkFileExtensionForBinary( string filePath ) ; 
		string remoteUrlToLocal( string url ) ; 

		*/
void ofxLoadingManager::setup (  string _localSavePath ) 
{
	localDirectoryPath = _localSavePath ; 
	localDirectory.createDirectory( _localSavePath , true , true ) ; 

	//There has GOT to be a better way to do this
	textFileExtensions.push_back( "xml" ) ; 
	textFileExtensions.push_back( "json" ) ; 
	textFileExtensions.push_back( "txt" ) ;
	textFileExtensions.push_back( "csv" ) ;

	binaryFileExtensions.push_back( "png" ) ; 
	binaryFileExtensions.push_back( "jpg" ) ;
	binaryFileExtensions.push_back( "jpeg" ) ;
	binaryFileExtensions.push_back( "gif" ) ;
	binaryFileExtensions.push_back( "mov" ) ;
	binaryFileExtensions.push_back( "mp4" ) ; 

	currentId = 0 ;
	numFileSavers = 10 ; 
	fileSavers = new ofxThreadedFileSaver * [ numFileSavers ] ; 
	
	fileSavers[0] = &saver0 ; 
	fileSavers[1] = &saver1 ; 
	fileSavers[2] = &saver2 ; 
	fileSavers[3] = &saver3 ; 
	fileSavers[4] = &saver4 ; 
	fileSavers[5] = &saver5 ; 
	fileSavers[6] = &saver6 ; 
	fileSavers[7] = &saver7 ; 
	fileSavers[8] = &saver8 ; 
	fileSavers[9] = &saver9 ; 

	ofRegisterURLNotification( this ) ;
}

void ofxLoadingManager::update ( ) 
{

}

void ofxLoadingManager::draw ( ) 
{

}

void ofxLoadingManager::loadURL( string _url ) 
{
	ofxAsyncLoadingData data ; 
	data.setup( _url , generateUniqueId() , checkFileExtensionForBinary( _url ) ) ; 

	asyncLoadData.push_back( data ) ; 
	ofLoadURLAsync( data.url , ofToString( data.uniqueId ) ) ; 
}

void ofxLoadingManager::urlResponse( ofHttpResponse & response ) 
{
	stringstream ss ; 
	
	ss << "URL Response : " << response.status << endl ; 
	ss << " of URL : " << response.request.name << endl ; 
    //ss << " data : " << response << endl ; 
    if ( response.status == 200  )
    {
		string editPath = response.request.url ; 
		int cachePathIndex = editPath.find_last_of( "/") ; 
		string cachePath = editPath.substr( cachePathIndex + 1 ) ; 

		int fileSaverIndex = getNextEmptyFileSaverIndex() ; 
		if ( fileSaverIndex != -1 ) 
		{
			int asyncDataIndex =  getAsyncDataIndexFromUrl( response.request.name ) ; 
			if ( asyncDataIndex > -1 && asyncDataIndex < asyncLoadData.size() ) 
			{
				fileSavers[ fileSaverIndex ]->saveFileToPath( "" , response.data , asyncLoadData[ asyncDataIndex ].bBinaryFile ) ; 
			}
		}
	}
	else
	{

	}

	ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
}

int ofxLoadingManager::getAsyncDataIndexFromUrl( string url ) 
{
	for ( int i = 0 ; i < asyncLoadData.size() ; i++ ) 
	{
		
		if ( asyncLoadData[i].url.compare( url ) == 0 )
		{
			return i ; 
		}
	}

	return -1 ; 
}
int ofxLoadingManager::getNextEmptyFileSaverIndex( ) 
{
	
	for ( int i = 0 ; i < numFileSavers ; i++ ) 
	{
		if ( !fileSavers[i]->getIsRunning()  ) 
		{
			return i ; 
		}
	}

	return -1 ; 
}

bool ofxLoadingManager::checkFileExtensionForBinary( string filePath )  
{
	int index = filePath.find_last_of( ".") ; 
	string fileExtension = ofToLower( filePath.substr( index + 1 ) ) ; 

	stringstream ss ; 
	ss << "filePath : " << filePath << endl ;
	ss << "file extension : " << fileExtension << endl ;
	
	for ( int i = 0 ; i < textFileExtensions.size() ; i++ ) 
	{
		if ( fileExtension.compare( textFileExtensions[i] ) == 0 ) 
		{
			ss << "filePath is : " << textFileExtensions[i] << " is a TEXT file" << endl ; 
			ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
			return false ; 
		}
	}

	for ( int i = 0 ; i < binaryFileExtensions.size() ; i++ ) 
	{
		if ( fileExtension.compare( binaryFileExtensions[i] ) == 0 ) 
		{
			ss << "filePath is : " << binaryFileExtensions[i] << " is a BINARY file" << endl ; 
			ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
			return true ; 
		}
	}

	ss << "file type '" << fileExtension << "' not found. Assuming it's a binary file." << endl ; 
	ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
	return true ; 
}
	
string ofxLoadingManager::remoteUrlToLocal( string url ) 
{
	int index = url.find_last_of( "/" ) ; 
	string localUrl = url.substr( index + 1 ) ; 

	return ( localDirectoryPath + "/" + localUrl )   ; 
}


int ofxLoadingManager::generateUniqueId()  
{
	currentId++ ; 
	return currentId ; 
}

	
	
