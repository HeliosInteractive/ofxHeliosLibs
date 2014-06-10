#pragma once

#include "ofMain.h"
#include "ofxAlphaStackItem.h"
class SlidingDoorRect : public ofxAlphaStackItem
{
	public :
		SlidingDoorRect( )
		{

		}

		ofColor color ;



		float radius ;
		float scaleWidth ;
		bool bHorizontal ;


        ofColor fillColor ;
		float fillAlpha ;
		ofColor strokeColor ;
        float strokeAlpha ;

		float frameLineWidth ;
        int lineWidth ;
		bool bDrawStroke ;
		
	
		void setup ( ofColor _fillColor , ofRectangle _bounds ,  bool _bDrawStroke = true , ofColor _strokeColor = ofColor::white , float strokeWidth = 3 )
		{
			ofxAlphaStackItem::setup() ; 
			bounds = ofRectangle ( 0 , 0 , 200 , 100 ) ;
			radius = 25.0f ;
			strokeColor = _strokeColor ; 
			bHorizontal= false ;
			strokeAlpha = 255.0f ;
			fillAlpha = 0.0f ;
			fillColor = _fillColor ; 
			frameLineWidth = 1.0f ;
			lineWidth= strokeWidth ;
			bDrawStroke = _bDrawStroke ; 
			setupBounds ( _bounds ) ;
		}



		void update ( )
		{
			 if ( bHorizontal == true )
			 {
				 bounds.height = ( bounds.height < origBounds.height ) ? origBounds.height : bounds.height ;
			 }
			 else
			 {
				 bounds.width = ( bounds.width < origBounds.width ) ? origBounds.width : bounds.width ;
			 }
		}

		void setupBounds ( ofRectangle rect )
		{
			origBounds  = rect ;
			bounds = rect ; 
		}

		void draw ( )
		{
			if ( alpha == 0 ) return ; 
			ofFill() ;
			ofEnableAlphaBlending( ) ;
			ofSetColor ( fillColor.r , fillColor.g , fillColor.b , getOFAlpha() ) ;
			roundedRect( bounds.x , bounds.y , bounds.width , bounds.height , radius ) ;
/*
			if ( bDrawStroke ) 
			{
				ofNoFill() ;
				ofSetColor ( strokeColor.r , strokeColor.g , strokeColor.b ,  getOFAlpha()  ) ;
				ofSetLineWidth( frameLineWidth ) ;
				roundedRect( bounds.x , bounds.y ,  bounds.width , bounds.height  , radius ) ;

				ofSetLineWidth( 1 ) ;
				ofFill( ) ;  
			}*/
			//ofSetColor ( 255 , 255 , 255 , 255 ) ;
		}

		
		void roundedRect(float x, float y, float w, float h, float r)
		{
			ofEnableSmoothing() ;
			ofSetLineWidth( lineWidth ) ;

			ofBeginShape() ;
				ofVertex(x+r, y);
				ofVertex(x+w-r, y);
				quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
				ofVertex(x+w, y+h-r);
				quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);

				ofVertex(x+r, y+h);
				quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
				ofVertex(x, y+r);
				quadraticBezierVertex(x, y, x+r, y, x, y+r);
			ofEndShape();
			ofSetLineWidth( 1 ) ;
			ofDisableSmoothing() ;

		}

		void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY)
		{
			float cp1x = prevX + 2.0/3.0*(cpx - prevX);
			float cp1y = prevY + 2.0/3.0*(cpy - prevY);
			float cp2x = cp1x + (x - prevX)/3.0;
			float cp2y = cp1y + (y - prevY)/3.0;

			// finally call cubic Bezier curve function
			ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
		};


		ofRectangle* getBoundsRef ( ) { return &bounds ; }
		ofRectangle getBounds ( ) { return bounds ; }
		ofRectangle getOriginalBounds ( ) { return origBounds ; }

		ofRectangle origBounds ;
		ofRectangle bounds ;

};
