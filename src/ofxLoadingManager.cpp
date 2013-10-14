#include "ofxLoadingManager.h"

void ofxLoadingManager::setup (  string _localSavePath ) 
{
	totalLoadTime = 0.0f; 
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
	ofAddListener( LoadingEvents::Instance()->FILE_LOADED , this, &ofxLoadingManager::loadCompleteHandler ); 
	

	for ( int i = 0 ; i < numFileSavers ; i++ ) 
	{
		ofAddListener( fileSavers[i]->timeoutTimer.TIMER_COMPLETE , this , &ofxLoadingManager::timerCompleteHandler ) ; 
	//	fileSavers[i]->update() ;  
	}

	checkFileLoopTimer.setup( 5000 , "checkFileLoopTimer" ) ;
	ofAddListener( checkFileLoopTimer.TIMER_COMPLETE , this , &ofxLoadingManager::checkFileLoopComplete ) ; 

	threadedLoader = new ofxThreadedImageLoader() ; 
	//ofxThreadedImageLoader * threadedLoader ; 
}



void ofxLoadingManager::update ( ) 
{

	//cacheDataQueue.push_back( asyncLoadData[ asyncDataIndex ] ) ;
	//removeAsyncDataFromQueue( asyncLoadData[ asyncDataIndex ] ) ; 
	checkFileLoopTimer.update() ; 

	for ( int i = 0 ; i < numFileSavers ; i++ ) 
	{
		fileSavers[i]->update() ;  
	}

}

void ofxLoadingManager::draw ( ) 
{
	ofBackground( 255 ) ; 
	int y = 20 ; 

	ofSetColor( 35 ) ; 
	ofColor color = ofColor( 0 , 179 , 221 ) ; 

	ofSetColor( color ) ; 
	ofPushMatrix() ;
		ofTranslate( 20 , 10 ) ;
		ofDrawBitmapString( "WAITING TO LOAD : " + ofToString( asyncLoadData.size() ) , 0 , 5 ) ; 
		ofTranslate( 0 , 30 ) ; 
		for ( int i = 0 ; i < asyncLoadData.size() ; i++ ) 
		{
			ofDrawBitmapString( remoteUrlToLocal( asyncLoadData[i].url ) , 0 , y * i ) ; 
		}
	ofPopMatrix() ; 

	ofPushMatrix() ;
		ofTranslate( 450 , 10 ) ;
		if ( asyncLoadData.size()  > 0 ) 
		{
			ofSetColor( 0 , 255 , 0 , 129.0f + ( sin ( ofGetElapsedTimef() * 9.0f ) * 128.0f ) ) ;  
			ofCircle( -10 , 0 , 8 ) ;
			ofSetColor( color ) ; 
			ofDrawBitmapString( "CURRENTLY LOADING : " + remoteUrlToLocal(  asyncLoadData[0].url ) , 0 , 5 ) ;
		}

		ofSetColor( color ) ; 
		ofTranslate( 0 , 30 ) ;
		ofDrawBitmapString( "WAITING TO WRITE: " + ofToString( cacheDataQueue.size() ) , 0 , 5 ) ;
		
		for ( int i = 0 ; i < cacheDataQueue.size() ; i++ ) 
		{
			ofDrawBitmapString(  remoteUrlToLocal(  cacheDataQueue[i].url ) , 0 , y * i ) ; 
		}
	ofPopMatrix() ; 

	ofPushMatrix() ;
		ofTranslate( 900 , 10 ) ;
		ofDrawBitmapString( "COMPLETED QUEUE : " + ofToString( completedData.size() ) , 0 , 5 ) ; 
		ofTranslate( 0 , 30 ) ; 
		for ( int i = 0 ; i < completedData.size() ; i++ ) 
		{
			ofDrawBitmapString(  remoteUrlToLocal(  completedData[i].url ) , 0 , y * i ) ; 
		}
	ofPopMatrix() ; 
}

void ofxLoadingManager::setupLoadingDirectory ( string loadDirPath ) 
{
	loadDirectoryPath = loadDirPath ; 
}


void ofxLoadingManager::checkFileLoopComplete ( int &args )  
{
	int count = 0 ; 
	bool bAdd = false ; 
	vector< ofxAsyncLoadingData > :: iterator d ; 
	for ( d = asyncLoadData.begin() ; d != asyncLoadData.end() ; d++ ) 
	{
			//cout << " w : " << (*threadedQueueImageRefs[ count ]).getWidth() << " , h : " << (*threadedQueueImageRefs[ count ]).getHeight() << endl ; 
			bAdd = addToThreadedImageQueue( (*threadedQueueImageRefs[ count ]) , (*d).url , false ) ; 
			//cout << "ATTEMPTING TO ADD : " << (*d).url << endl ; 
			if ( bAdd == true ) 
			{
				ofLogVerbose( "it now exists ! LOADING" ) ; //<< endl ;
				d = asyncLoadData.erase( d ) ; 
				return ; 
			}
		
		count ++ ; 
	}


	int refCount = 0 ; 
	vector< ofImage * > :: iterator ref ; 
	for ( ref = threadedQueueImageRefs.begin() ; ref != threadedQueueImageRefs.end() ; ref++ ) 
	{

		if ( bAdd == true && refCount == count ) 
		{
			ref = threadedQueueImageRefs.erase( ref ) ; 
		}
		refCount++ ; 
	}
}


//There's a WAY better way to implement this.
bool ofxLoadingManager::addToThreadedImageQueue( ofImage & image , string path , bool bAddToQueue ) 
{
	char slashChar = '/' ;
	if ( path[0] == slashChar ) 
	{
		//cout << "GET THAT SLASH OUT OF THERE!" <<endl; 
		path = path.substr( 1 , path.size() -1 ) ; 
		//cout << "path is now : " << path << endl ;

	}
	string localString = loadDirectoryPath + remoteUrlToLocal( path ) ; 
	//cout << "directoryPath" << loadDirectoryPath << endl ; 
	//cout << "path :" << path << " localPath : " << localString << endl ; 

	

	bool bBinary = checkFileExtensionForBinary( path  ) ; 
	ofFile checkIfItLocallyExists ; //ofFile(localString,ofFile::ReadOnly , bBinary ) ;  
	bool bExists = checkIfItLocallyExists.doesFileExist( localString , true ) ; 

	stringstream ss ; 

	ofxAsyncLoadingData data ; 
	data.setup( localString , generateUniqueId() , bBinary ) ; 
	if ( bExists ) 
	{
		ss << "'" << path << "'" << " already exists ! loading now" << endl ; 
		ofLogVerbose( ss.str() ) ;

		//TODO do local things and notify that the file is ready to be loaded.
		threadedLoader->loadFromDisk( image , data.url ) ;  
		
		return true  ; 
	}
	else
	{
		ss << "bAddToQueue :" << bAddToQueue << "  - '" << path << "' does not exist, adding to the load queue" << endl ; 
		ofLogVerbose( ss.str() ) ;
		if ( bAddToQueue == true ) 
		{
			threadedQueueImageRefs.push_back( &image ); 
			asyncLoadData.push_back( data ) ; 
			checkFileLoopTimer.start( true , false ) ; 
		}
		
		return false ; 
	}
	
	 

}

void ofxLoadingManager::loadURL( string _url   ) 
{
	string localString = remoteUrlToLocal( _url ) ; 

	bool bBinary = checkFileExtensionForBinary( _url ) ; 
	ofFile checkIfItLocallyExists ; //ofFile(localString,ofFile::ReadOnly , bBinary ) ;  
	bool bExists =checkIfItLocallyExists.doesFileExist( localString , true ) ; 

	stringstream ss ; 
	if ( bExists ) 
	{
		ss << "'" << localString << "'" << " already exists ! No need to load it locally." << endl ; 
		//TODO do local things and notify that the file is ready to be loaded.

	}
	else
	{
		ss << "'" << localString << "' does not exist, adding to the load queue" << endl ; 
		ofxAsyncLoadingData data ; 
		data.setup( _url , generateUniqueId() , checkFileExtensionForBinary( _url ) ) ; 

		asyncLoadData.push_back( data ) ; 
		ofLoadURLAsync( data.url , ofToString( data.uniqueId ) ) ; 
	}
	
	ofLogVerbose( ss.str() ) ; 
}

void ofxLoadingManager::urlResponse( ofHttpResponse & response ) 
{
	stringstream ss ; 
	
	ss << "URL Response : " << response.status << endl ; 
	ss << " of URL : " << response.request.url << endl ; 
    //ss << " data : " << response << endl ; 

	ofxAsyncLoadingData myData ; 
	int asyncDataIndex =  getAsyncDataIndexFromUrl( response.request.url ) ; 
	if ( asyncDataIndex > -1 && asyncDataIndex < asyncLoadData.size() ) 
	{
		myData = asyncLoadData[ asyncDataIndex ]; 
	}
	else
	{
		ofLogVerbose( "COULD NOT MATCH URL to an async data" ) ; 
		return ; 
	}

	if ( response.status == 200  )
    {
		string editPath = response.request.url ; 
		int cachePathIndex = editPath.find_last_of( "/") ; 
		string cachePath = editPath.substr( cachePathIndex + 1 ) ; 

		//ofxAsyncLoadingData myData = asyncLoadData[ asyncDataIndex ]; 
		int fileSaverIndex = getNextEmptyFileSaverIndex() ; 
		if ( fileSaverIndex != -1 ) 
		{
			//ss << "fileSaverIndex : " << fileSaverIndex << endl; 
			//ss << "asyncDataIndex : " << asyncDataIndex << endl; 
			myData.totalLoadTime = ofGetSystemTime() - myData.startLoadTime ; 
			totalLoadTime += myData.totalLoadTime ; 
			string localPath = remoteUrlToLocal( myData.url ) ; 
			fileSavers[ fileSaverIndex ]->saveFileToPath( localPath , response.data , myData.bBinaryFile , response.request.url ) ; 
			writingData.push_back( myData ) ; 
			removeAsyncDataFromQueue( myData ) ; 
			if ( asyncLoadData.size() > 0 ) 
				asyncLoadData[0].startLoadTime = ofGetSystemTime(); 
		}
		else
		{
			ss << "no file savers are available.. adding to cacheDataQueue" << endl;
			cacheDataQueue.push_back( asyncLoadData[ asyncDataIndex ] ) ;
			removeAsyncDataFromQueue( asyncLoadData[ asyncDataIndex ] ) ; 
		}
	}
	else
	{
		ss << "file did not load. Should RELOAD or RETRY loading it for best results..." << endl; 
		loadURL( myData.url ) ;
	}

	ofLog( OF_LOG_WARNING , ss.str() ) ; 
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
		if ( !fileSavers[i]->isThreadRunning()  ) 
		{
			return i ; 
		}
	}

	return -1 ; 
}

void ofxLoadingManager::removeAsyncDataFromQueue( ofxAsyncLoadingData data )  
{
	vector< ofxAsyncLoadingData >::iterator d = asyncLoadData.begin() ; 

	for ( d ; d != asyncLoadData.end() ; ++d ) 
	{
		if ( data.url.compare( (*d).url ) == 0 ) 
		{
			asyncLoadData.erase( d ) ; 
			return ; 
		}
	}
}


bool ofxLoadingManager::checkFileExtensionForBinary( string filePath )  
{
	int index = filePath.find_last_of( ".") ; 
	string fileExtension = ofToLower( filePath.substr( index + 1 ) ) ; 

	stringstream ss ; 
	//ss << "filePath : " << filePath << endl ;
	//ss << "file extension : " << fileExtension << endl ;
	
	for ( int i = 0 ; i < textFileExtensions.size() ; i++ ) 
	{
		if ( fileExtension.compare( textFileExtensions[i] ) == 0 ) 
		{
			//ss << "filePath is : " << textFileExtensions[i] << " is a TEXT file" << endl ; 
			//ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
			return false ; 
		}
	}

	for ( int i = 0 ; i < binaryFileExtensions.size() ; i++ ) 
	{
		if ( fileExtension.compare( binaryFileExtensions[i] ) == 0 ) 
		{
			//ss << "filePath is : " << binaryFileExtensions[i] << " is a BINARY file" << endl ; 
			//ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
			return true ; 
		}
	}

	//ss << "file type '" << fileExtension << "' not found. Assuming it's a binary file." << endl ; 
	//ofLog( OF_LOG_VERBOSE , ss.str() ) ; 
	return true ; 
}
	
string ofxLoadingManager::remoteUrlToLocal( string url ) 
{
	int index = url.find_last_of( "/" ) ; 
	string localUrl = url.substr( index + 1 ) ; 

	return ( localDirectoryPath + '/' + localUrl )   ; 
}


int ofxLoadingManager::generateUniqueId()  
{
	currentId++ ; 
	return currentId ; 
}

void ofxLoadingManager::timerCompleteHandler( int &args ) 
{
	/*
	ofxLoadingManager
	*/
	if ( args < numFileSavers ) 
	{
		fileSavers[args]->retry() ; 
	}
}


void ofxLoadingManager::loadCompleteHandler( string &args )  
{
	vector< ofxAsyncLoadingData >::iterator d = writingData.begin() ; 

	for ( d ; d != writingData.end() ; ++d ) 
	{
		if ( args.compare( (*d).url ) == 0 ) 
		{
			completedData.push_back( (*d) ) ; 
			writingData.erase( d ) ; 

			return ; 
		}
	}

	//ofSetLogLevel ( OF_LOG_WARNING ) ; 
	cout <<  args << " : has finished writing to disk." << endl ;  
}

void ofxLoadingManager::stop ( ) 
{
	threadedLoader->yield() ; 
	threadedLoader->waitForThread( true ) ; 
	delete threadedLoader ; 
	threadedLoader = NULL ; 

	asyncLoadData.clear( ) ; 

	ofUnregisterURLNotification( this ) ; 
	
	checkFileLoopTimer.stop() ;
	threadedQueueImageRefs.clear( ) ; 

	for ( int i = 0 ; i < numFileSavers ; i++ ) 
	{
		ofRemoveListener( fileSavers[i]->timeoutTimer.TIMER_COMPLETE , this , &ofxLoadingManager::timerCompleteHandler ) ; 
		fileSavers[i]->waitForThread( true ) ; 
	}

	ofRemoveListener( checkFileLoopTimer.TIMER_COMPLETE , this , &ofxLoadingManager::checkFileLoopComplete ) ; 
	ofRemoveAllURLRequests( ) ; 

}

	
	
