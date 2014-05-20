
#pragma once

#include "ofMain.h"
#include "MultiTouchCursor.h"

class MultiTouchActions {

	public:
		void setup();
		void update();
		void draw();

		void touchDown(MultiTouchCursor & tcur);
        void touchDrag(MultiTouchCursor & tcur);
        void touchUp(MultiTouchCursor & tcur);
		
		void touchDown( ofPoint p , int id ) ; 
		void touchDrag( ofPoint p , int id ) ; 
		void touchUp( ofPoint p , int id ) ; 

		void touchDown( float x , float y , int id ) { touchDown( ofPoint ( x , y ) , id ) ; } 
		void touchDrag( float x , float y , int id ) { touchDrag( ofPoint ( x , y ) , id ) ; } 
		void touchUp( float x , float y , int id )   { touchUp( ofPoint ( x , y ) , id ) ; } 

        void reset();

		ofPoint delta ; 
		float deltaDeg ; 

        //ofPoint normal ; 
        ofPoint calcNormal ;
		ofPoint storedNormal ; 
		ofPoint startNormal ; 

		ofPoint getNormal( ) ;

		float scale ; 

		void rotateToNormal(ofVec3f normal) ; 

        void setScaleRange( float _minScale , float _maxScale )
        {
            minScale = _minScale ;
            maxScale = _maxScale ;
        }

        float getMinScale( ) { return minScale ; }
        float getMaxScale( ) { return maxScale ; }

        vector <MultiTouchCursor> objTouch;
		ofPoint offset ; 

    protected:

    private:
        float cursorTime, elapsedTime;
		ofPoint last , origin, calc ;
		/*
        float lastX, lastY, lastZ, orgX, orgY, orgZ ;
        float calcX, calcY, calcZ ;
        float offsetX, offsetY, offsetZ ; */
        float lastDist;
        int w, h;

        float maxScale , minScale ;

        int firstPass, fingerAdded, fingerRemoved, isLerp, firstScale;

        vector < ofPoint > fAngles;

        void updateTranslate();
        void updateRotation();
        void updateTime();
        void updateScale();
};
