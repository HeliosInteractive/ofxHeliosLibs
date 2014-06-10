#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "BasePolyObject.h"
#include "ofxTextSuite.h"
#include "ofxPolygonHitTest.h"
#include "AlphaStackItem.h"

class TextField : public BasePolyObject , public AlphaStackItem
{
    public:
        TextField() {}
        virtual ~TextField() {}

        float x , y ;
        ofColor color ;

        bool bActive ;      //If the text field is currently listening or being forwarded input events
        float antiAliasFactor ;
		string text ;

        ofColor activeColor ;
        ofRectangle bounds ;

    virtual void init ( float _x , float _y , float _width , float _height , string fontPath , int fontSize , string _defaultText , ofColor _color = ofColor::white , ofColor _activeColor = ofColor::red , float antiAlias = 4.0f )
        {
            
            activeColor = _activeColor ; 
            antiAliasFactor = antiAlias ;
            textBlock.init( fontPath , fontSize , antiAliasFactor ) ;
            defaultText = _defaultText ;
			setText( defaultText ) ;
            //textBlock.setText( defaultText ) ;
            x = _x ;
            y = _y ;
            color = _color ;

            bActive = false ;
            activeText = "";

            bounds = ofRectangle( _x , _y , _width , _height ) ; 
            addPoint( x , y ) ;
            addPoint ( x + _width , y ) ;
            addPoint ( x + _width , y + _height ) ;
            addPoint ( x , y + _height ) ;
            
            
        }

		virtual void setText ( string _text ) 
		{
			text = _text ;
			textBlock.setText( _text ) ;
		}


        virtual void appendText ( string _addition )
        {
            if ( !bActive )
                return ;

            if ( activeText == "" )
            {
                activeText = _addition ;
            }
            else
            {
                //TODO : max character limit ?
                activeText += _addition ;
            }

             textBlock.setText( activeText ) ;
            text = activeText ;
        }

        virtual void backspace ( )
        {
            if ( !bActive )
                return ;

            if ( activeText.size() > 1 )
            {
                string str = activeText.substr( 0 , activeText.size() - 1 ) ;
                activeText = str ;
                textBlock.setText( activeText ) ;
            }
            else
            {
                activeText = "" ;
                textBlock.setText( defaultText ) ;
            }
            
            text = activeText ;
        }

        virtual void setActive ( bool newActive )
        {
            if( newActive == bActive )
                return ;

            bActive = newActive ;
            if ( bActive == true )
            {
                if ( activeText != "" )
                    textBlock.setText( activeText ) ;
                else
                    textBlock.setText( defaultText ) ;
            }
            else
            {
                textBlock.setText( defaultText ) ;
            }
        }

        virtual void update ( )
        {

        }
    
        virtual bool input ( float x , float y )
        {
            bool bResult = ofxPolygonHitTest::Instance()->pointInPoly( pts , x , y ) ;
            if ( bResult == true )
            {
                cout << "SHOULD NOW BE ACTIVE!" << endl ;
                bActive = true ;
                return true ;
            }

            return bResult ;
        }
    
        virtual void draw ( )
        {
            //baseDraw( ) ;
             ofSetColor( color , getOFAlpha() * .25 ) ;
            if ( bActive == true )
            {
                ofSetColor ( color , getOFAlpha() ) ;
               
               // ofRect( bounds ) ;
            }
            //ofNoFill( ) ;
            
            //ofRect( bounds ) ;
            ofFill( ) ; 
            
            ofPushMatrix( ) ;
                ofTranslate( x , y ) ;
                textBlock.draw( 0 , 0 ) ;
            ofPopMatrix( ) ;
        }

		virtual void drawCenter ( )
        {
			ofSetColor ( color , getOFAlpha() ) ;
            ofPushMatrix( ) ;
                ofTranslate( x , y ) ;
                textBlock.drawCenter( 0 , 0 ) ;
            ofPopMatrix( ) ;
        }

        ofxTextBlock textBlock ;
        string defaultText ;
         string activeText ;
    protected:

    private:
};

#endif // TEXTFIELD_H
