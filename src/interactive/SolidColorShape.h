//
//  SolidColorShape.h
//  MWV_CES
//
//  Created by Helios on 12/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MWV_CES_SolidColorShape_h
#define MWV_CES_SolidColorShape_h

#include "ofMain.h"
#include "ofxTweenzor.h"

class SolidColorShape
{
    public :

        SolidColorShape()
        {
            alpha = 255.0f ;
        }

        void setup ( ofColor _color , float _xOffsetUp , float _xOffsetDown , float _centerOffset , float _skewRatio )
        {
            xOffsetUp = _xOffsetUp ;
            xOffsetDown = _xOffsetDown ;
            centerOffset = _centerOffset ;
            skewRatio = _skewRatio ;

            /*
            float xOffsetUp = -10 ;
            float xOffsetDown = 10 ;

            float centerOffset = 10 ;
            float skewRatio = 0.5f ; */
            color = _color ;
            totalAnimationTime = 1.0f + 1.8f + 0.5f + 0.15f ;
        }

        void transitionIn ( ofVec2f position )
        {


            corners.clear() ;
            //top left
            corners.push_back ( ofVec2f ( position.x + xOffsetUp - centerOffset * skewRatio , position.y - centerOffset ) ) ;
            //top right
            corners.push_back ( ofVec2f ( position.x + xOffsetUp + centerOffset * skewRatio , position.y - centerOffset ) ) ;
            //bottom right
            corners.push_back ( ofVec2f ( position.x + xOffsetDown + centerOffset * skewRatio , position.y + centerOffset ) ) ;
            //bottom left
            corners.push_back ( ofVec2f ( position.x + xOffsetDown - centerOffset * skewRatio , position.y + centerOffset ) ) ;

            alpha = 0.0f ;
            Tweenzor::add ( &alpha , alpha , 255.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ;
            float tweenTime = 0.1f ;


            Tweenzor::add ( &corners[0].x , corners[0].x , corners[0].x - corners[0].y * skewRatio - centerOffset , 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[0].y , corners[0].y , 0.0f , 0.0f , tweenTime , EASE_IN_QUAD ) ;

            Tweenzor::add ( &corners[1].x , corners[1].x , corners[1].x - corners[1].y * skewRatio + centerOffset, 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[1].y , corners[1].y , 0.0f , 0.0f , tweenTime , EASE_IN_QUAD ) ;

            Tweenzor::add ( &corners[2].x , corners[2].x , corners[2].x + ( ofGetHeight() - corners[2].y ) * skewRatio + centerOffset, 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[2].y , corners[2].y , ofGetHeight() , 0.0f , tweenTime , EASE_IN_QUAD ) ;

            Tweenzor::add ( &corners[3].x , corners[3].x , corners[3].x + ( ofGetHeight() - corners[3].y ) * skewRatio - centerOffset , 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[3].y , corners[3].y , ofGetHeight() , 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::addCompleteListener ( Tweenzor::getTween( &corners[3].y ) , this , &SolidColorShape::phase2 ) ;

        }

        void phase2 ( float * args )
        {
            float tweenTime = 1.0f ;
            Tweenzor::add ( &corners[0].x , corners[0].x , corners[0].x - 3500 , 0.0f , tweenTime , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[1].x , corners[1].x , corners[1].x + 3500 , 0.0f , tweenTime/4 , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[2].x , corners[2].x , corners[2].x + 3500 , 0.0f , tweenTime/4 , EASE_IN_QUAD ) ;
            Tweenzor::add ( &corners[3].x , corners[3].x , corners[3].x - 3500 , 0.0f , tweenTime , EASE_IN_QUAD ) ;

            Tweenzor::addCompleteListener ( Tweenzor::getTween ( &corners[3].x ) , this , &SolidColorShape::phase3 ) ;
        }

        void phase3 ( float * args )
        {
            Tweenzor::add ( &alpha , alpha , 0.0f , 0.0f , 0.5f , EASE_IN_QUAD ) ;
            Tweenzor::addCompleteListener ( Tweenzor::getTween ( &alpha ) , this , &SolidColorShape::phase3Complete ) ;

            int irrelevant = 4 ;
            ofNotifyEvent ( SolidColorShape::Instance()->IDLE_SHAPE_TAKEOVER_COMPLETE , irrelevant , this ) ;
        }

        void phase3Complete ( float * args )
        {
            int irrelevant = 4 ;
            ofNotifyEvent ( SolidColorShape::Instance()->IDLE_SHAPE_OUT_COMPLETE , irrelevant , this ) ;

        }

        void draw ( )
        {
            ofSetColor( color.r , color.g , color.b , alpha ) ;
            ofBeginShape() ;
                for ( int i = 0 ; i < corners.size() ; i++ )
                {
                    ofVertex( corners[i].x , corners[i].y ) ;
                }
            ofEndShape() ;
        }

        float totalAnimationTime ;

        float xOffsetUp ;
        float xOffsetDown ;

        float centerOffset ;
        float skewRatio ;
        float alpha ;

        static SolidColorShape* Instance()
        {
            static SolidColorShape inst ;
            return &inst ;
        }

        ofEvent<int> IDLE_SHAPE_TAKEOVER_COMPLETE ;
        ofEvent<int> IDLE_SHAPE_OUT_COMPLETE ;

    private :

        ofColor color ;
        vector<ofVec2f> corners ;

};


#endif
