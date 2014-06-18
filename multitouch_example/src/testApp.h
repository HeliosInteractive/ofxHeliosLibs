#pragma once

#include "ofMain.h"
#include "MultiTouchCursor.h"
#include "MultiTouchObject.h"
#include "ofBaseTouchApp.h"

//If you don't have access to a touch screen you can debug rotation / scale with the mouse
//#define NO_MULTITOUCH 3

class testApp : public ofBaseTouchApp {
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		MultiTouchObject multiTouchObject ;

		// win7 events
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);

		// quick map to store current touches
		map<int,ofPoint> touches;

#ifdef NO_MULTITOUCH
		MultiTouchCursor mouseDrag ;
		MultiTouchCursor mouseDragReflect ;
#endif 

};
