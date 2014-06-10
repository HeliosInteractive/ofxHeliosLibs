#ifndef TEXTFBO_H
#define TEXTFBO_H

#include "ofMain.h"
#include "FontObject.h"
#include "ofxTextBlock.h"

class TextFBO
{
    public:
        TextFBO( )
        {
            bRasterized = false ;
            rawText = "noRawText" ;
        }
        virtual ~TextFBO() {}

        ofFbo textFbo ;
        string rawText ;
        bool bRasterized ;
        ofxTextBlock textBlock ;

        float alpha ;

        //FontObject titleFO = FontManager::Instance()->getFontByName( FontList::Instance()->TEST0 ) ;

        void setText ( string text , FontObject fo , float width ,float height )
        {
            if ( bRasterized == true )
                clear() ;

			//textBlock.init ("type/testFont_1.otf" , 15 ) ;
			textBlock.init( fo.path , fo.size ) ;
            textBlock.setText( text ) ;
            rawText = text ;
            textBlock.wrapTextX( width + 25 ) ;
        //    textBlock.wrapTextArea( width , height ) ;
// "type/testFont_1.otf"
            rasterizePadding = 5 ;

            renderText() ;
           // cout << "text is ! " << text << endl ;
            bRasterized = true ;
            alpha = 255.0f ;
        }

        void renderText( )
        {
            bRasterized = true ;

            int w = textBlock.getWidth() + rasterizePadding * 2 ;
            int h = textBlock.getHeight() + rasterizePadding * 2 ;

            textFbo.allocate( w , h , GL_RGBA ) ;
            ofEnableAlphaBlending() ;
			textFbo.begin() ;
                ofClear(0, 0, 0, 1);
				ofPushMatrix() ;
					ofTranslate ( rasterizePadding , rasterizePadding , 0 ) ;
                    ofSetColor ( 255 , 255 , 255 ) ;


                    textBlock.drawCrispLeft( 0 , 0 ) ;
             //   ofDisableSmoothing() ;
				//ofSetColor( 255 , 0, 0 ) ;
				//ofRect( 0 , 0, 200 , 200 ) ;
				//ofPopMatrix() ;
			textFbo.end() ;
        }

        void clear( )
        {

        }

        int getWidth ( )
        {
            return textFbo.getWidth( ) ;
        }

        int getHeight( )
        {
            return textFbo.getHeight( ) ;
        }
        void draw ( float x , float y )
        {
            if ( !bRasterized ) return ;

           // ofSetColor( 255 , 255 , 255 , alpha ) ;
        //    cout << " w : " << textFbo.getWidth() << " , " << " h : " << textFbo.getHeight() << endl ;
           textFbo.draw( x , y ) ;
           //textBlock.draw( x , y ) ;
           //cout << "text is : " << textBlock.rawText << endl ;

        }
        int rasterizePadding ;

    protected:
    private:
};

#endif // TEXTFBO_H
