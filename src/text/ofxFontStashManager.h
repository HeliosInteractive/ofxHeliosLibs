#pragma once

#include "ofMain.h"
#include "ofUnicode.h"
#include <unordered_map>
#include "ofxFontStash.h"

class ofxFontStashManager
{
public : 
	
	static ofxFontStash * getFont ( string fontPath ) ;

	static ofxFontStashManager* Instance()
	{
		initInstance();
        return instance.get();
	}
	
	~ofxFontStashManager();
	void exportToXML( string xmlName = "fontExport.xml" ) ;

private : 
	ofxFontStashManager();
	static void initInstance();
	static std::auto_ptr< ofxFontStashManager > instance;
	unordered_map< string,ofxFontStash *> fontTable;
    ofxFontStashManager( ofxFontStashManager const& );          
    void operator=(ofxFontStashManager const&);
	ofxFontStash * getFontInternal(string fontPath );
};

