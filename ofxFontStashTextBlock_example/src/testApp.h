#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"
#include "ofxFontStashTextBlock.h"

//comment this line to ignore "ofxTimeMeasurements" calls
//enabling it requires the ofxTimeMeasurements addon
//https://github.com/armadillu/ofxTimeMeasurements
//#define MEASURE_PERFORMANCE

class testApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		//ofxFontStash unicodeFont;
		ofxFontStashTextBlock textBlock1 ; 
		ofxFontStashTextBlock textBlock2 ; 
};
