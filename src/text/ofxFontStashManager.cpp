#include "ofxFontStashManager.h"

std::auto_ptr<ofxFontStashManager> ofxFontStashManager::instance;

ofxFontStashManager::ofxFontStashManager(){} 
ofxFontStashManager::~ofxFontStashManager(){ofLogNotice() << "The instance of FTGLFontManager was destroyed";} 

void ofxFontStashManager::initInstance ()
{
	if (!instance.get())
	{
		ofLogNotice("FontManager") << "The instance of FTGLFontManager was created";
		instance.reset( new ofxFontStashManager() );
	}
}


ofxFontStash * ofxFontStashManager::getFont ( string fontPath )
{
	return Instance()->getFontInternal(fontPath );
}

ofxFontStash * ofxFontStashManager::getFontInternal ( string fontPath )
{
	//Let's make sure our font path is relative to the data folder
	string localPath = ofToDataPath( fontPath , false ) ; 

	//See if the path is formatted to use backslashse
	int lastSlashIndex = fontPath.find_last_of( "/" );

	//No backslahses ? It must be using forward slashes
	if ( lastSlashIndex < 0 )
		lastSlashIndex = fontPath.find_last_of( "\\" ) ; 
	else if ( lastSlashIndex < 0 )
		localPath = fontPath.substr( lastSlashIndex  + 1 ) ; 
	
	string fontkey = localPath ; //+ofToString(fontSize);

	if (fontTable.find(fontkey) == fontTable.end())
	{
		ofxFontStash* newFont = new ofxFontStash();
		newFont->setup( localPath , 1.0f ); 
		fontTable[fontkey] = newFont;
		ofLogNotice("Font Cached") << fontPath<< " @ "<<fontTable.size()<<" fonts cached total.";
	}

	return fontTable[fontkey];
} 