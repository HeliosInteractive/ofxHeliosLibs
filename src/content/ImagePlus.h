#pragma once

#include "ofxTweenzor.h"
#include "ofxMatrixTransformObject.h"

class ImagePlus : public ofxMatrixTransformObject
{
    public:

    float alpha ;

        ImagePlus() {
            scale = 1.0f ;
            alpha = 1.0f ;
            rotation = 0.0f;
            inheritAlpha = false ;
            lastInputTime = -2.0f ;
            inputTimeDelay = 0.5f ;
        }

        void init ( string path ,  ofVec2f _position , float _rotation = 0 , float anchorX = 0.0f , float anchorY = 0.0f )
        {
			//AppConstants::Instance()->loader.loadFromDisk( image , path ) ; 
            //image.loadImage ( path ) ;
           // cout << "anchor percent : " << anchorX << " , " << anchorY << endl ;
            image.setAnchorPercent( anchorX , anchorY ) ;
            scale = 1.0f ;
            alpha = 1.0f ;
            rotation = _rotation ;
            x = _position.x ;
            y = _position.y ;
            inheritAlpha = false ;
            color = ofColor::white ;
            lastInputTime = 0.0f ;
            inputTimeDelay = 0.5f ;
        }

        bool timeInputThreshold( )
        {
            bool result = ( ofGetElapsedTimef() >= ( lastInputTime + inputTimeDelay ) ) ;
            if ( result == true )
                lastInputTime = ofGetElapsedTimef() ;
            return result ;
        }

        void setInputDelay ( float inputDelay )
        {
            inputTimeDelay = inputDelay ;
        }

        void draw( )
        {
			pushMatrix() ;
                if ( inheritAlpha == false )
                {
                    ofEnableAlphaBlending() ;
                    ofSetColor ( color.r , color.g , color.b , alpha * 255.0f ) ;
                }
                //ofRect( 0 , 0, hitBounds.width , hitBounds.height ) ;
                image.draw (  0 , 0 ) ;

            popMatrix() ;
        }



        void transitionIn( float time = 0.25f , float delay = 0.0f )
        {
            Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
            Tweenzor::add( &scale , scale , 1.0f , delay , time , EASE_OUT_QUAD ) ;
        }

        void transitionOut( float time = 0.25f , float delay = 0.0f )
        {
            Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_IN_QUAD ) ;
            Tweenzor::add( &scale , scale , 0.01f , delay , time , EASE_IN_QUAD ) ;
        }

        virtual ~ImagePlus(){}

        ofImage image ;
        ofColor color ;
        bool inheritAlpha ;

        float lastInputTime ;
        float inputTimeDelay ;
    protected:
    private:
};

