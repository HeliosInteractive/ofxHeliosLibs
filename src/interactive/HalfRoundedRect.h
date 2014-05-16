#ifndef HALFROUNDEDRECT_H
#define HALFROUNDEDRECT_H


#include "ofMain.h"
#include "ofxTweenzor.h"


class HalfRoundedRect
{
    public:

        HalfRoundedRect()
        {
        }
        virtual ~HalfRoundedRect() {}

        float frameLineWidth ;
        ofRectangle originalBounds ;
        ofRectangle halfwayBounds ;

        void setup( float _x , float _y , float _radius , float _width , float _height )
        {
            x = _x ;
            y = _y ;
            radius = _radius ;
            width = _width ;
            height = _height ;

            originalBounds = ofRectangle ( x , y , width , height ) ;

            ofPoint north = ofPoint ( x , y - radius ) ;
            ofPoint east = ofPoint ( x + radius , y ) ;
            ofPoint south = ofPoint ( x , y + radius ) ;
            ofPoint west = ofPoint ( x - radius , y ) ;

            vertices.push_back( north ) ;
            vertices.push_back( north ) ;
            vertices.push_back( east ) ;
            vertices.push_back( east ) ;
            vertices.push_back( south ) ;
            vertices.push_back( south ) ;
            vertices.push_back( west ) ;
            vertices.push_back( west ) ;

            strokeColor = ofColor ( 255 , 0 , 0 , 90 ) ;
            fillColor = ofColor ( 255 , 255 , 255 , 125 ) ;
            strokeAlpha = 90 ;
            fillAlpha = 125 ;

            frameLineWidth = 1.0f ;

         //   anchorPerc = ofPoint ( 0.5 , 0.5 ) ;

        }
        vector<ofPoint> vertices ;
        ofColor strokeColor ;
        ofColor fillColor ;

        float strokeAlpha ;
        float fillAlpha ;

//        ofPoint anchorPerc ;

        void draw ( )
		{

         //   ofPushMatrix() ;
          //      ofTranslate( width * -anchorPerc.x , height * -anchorPerc.y , 0 ) ;


            ofFill() ;
            ofEnableAlphaBlending( ) ;
            ofSetColor ( fillColor.r , fillColor.g , fillColor.b , fillAlpha ) ;
            if ( fillAlpha > 0 )
            roundedRect ( x , y , width , height , radius ) ;


            ofSetLineWidth( frameLineWidth ) ;
            /*
            ofNoFill() ;
            ofSetColor ( strokeColor.r , strokeColor.g , strokeColor.b , strokeAlpha ) ;
            if ( strokeAlpha > 0 )
                roundedRect ( x , y , width , height , radius ) ;
*/
            ofSetLineWidth( 1.0f ) ;
                ofFill( ) ;
                ofSetColor ( 255 , 255 , 255 , 255 ) ;
            //ofPopMatrix();

		}

		void roundedRect(float x, float y, float w, float h, float r)
		{
			ofEnableSmoothing() ;
			ofSetLineWidth( 3 ) ;

            //top right and bottom left are straight, others are rounded
			ofBeginShape() ;
				ofVertex(x+r, y);
				ofVertex(x+w, y);
				//quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
				ofVertex(x+w, y+h-r);
				quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);

				ofVertex(x, y+h);
				//quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
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


        float x , y ;
        float radius ;      //COrner radius
        float width , height ;

    protected:
    private:
};

#endif // TWEENSHAPE_H
