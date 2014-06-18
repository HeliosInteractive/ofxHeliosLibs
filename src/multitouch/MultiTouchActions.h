
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

		void touchDown( float x , float y , int id )
		{
			touchDown( MultiTouchCursor( x , y , id ) ) ; 
		}
        void touchDrag( float x , float y , int id )
		{
			touchDrag( MultiTouchCursor( x , y , id ) ) ; 
		}

        void touchUp( float x , float y , int id  )
		{
			touchUp( MultiTouchCursor( x , y , id ) ) ; 
		}

        void reset();

        float deltaX, deltaY, deltaDeg;
        float scaleX, scaleY;


        void setScaleRange( float _minScale , float _maxScale )
        {
            minScale = _minScale ;
            maxScale = _maxScale ;
        }

        float getMinScale( ) { return minScale ; }
        float getMaxScale( ) { return maxScale ; }

        vector <MultiTouchCursor> objTouch;

    protected:

    private:
        float cursorTime, elapsedTime;
        float lastX, lastY, orgX, orgY;
        float calcX, calcY;
        float offsetX, offsetY;
        float lastDist;
        int w, h;

        float maxScale , minScale ;

        int firstPass, fingerAdded, fingerRemoved, isLerp, firstScale;

        vector <ofVec2f> fAngles;

        void updateTranslate();
        void updateRotation();
        void updateTime();
        void updateScale();
};