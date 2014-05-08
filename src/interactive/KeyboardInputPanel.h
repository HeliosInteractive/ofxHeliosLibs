#pragma once

#include "ofMain.h"
#include "ofxMatrixTransformObject.h"
#include "ofxAlphaStackItem.h"
#include "ofxTextSuite.h"

class KeyboardInputPanel : public ofxMatrixTransformObject , public ofxAlphaStackItem
{
public : 
	KeyboardInputPanel( ) { } 
	~KeyboardInputPanel( ) { } 

	ofImage image ; 
	ofxTextBlock inputBlock ;
	string defaultText ; 
	bool bTypingActive ; 
	int maxChar ;
	int fontX , fontY ; 

	string startText ; 
	string text ; 

	ofxMatrixTransformImage validateImage ; 

	int maxCharacters ; 
	ofFbo * compositeFbo ; 
		
	float fboScale ; 
	ofPoint fboPosition ; 

	ofEvent< string > SUBMIT_DATA ; 
	string storedText ; 


	ofxTextBlock scoreBlock ; 
	string endText ; 


	bool bShiftActive ; 

	void setup ( string imagePath , float _x , float _y , string fontPath , float fontSize , int _fontX , int _fontY ) 
	{
		fboScale = 1.0f ; 
		fboPosition = ofPoint( ) ; 
		compositeFbo = NULL ; 
		bool bImageLoadResult = image.loadImage( imagePath ) ; 

		defaultText = "PLEASE ENTER YOUR EMAIL ADDRESS" ; 
		ofLogVerbose( " did image load from ? " ) << imagePath << " ? " << bImageLoadResult << endl ; 
		inputBlock.init( fontPath , fontSize , ofColor( 108 , 108 , 108 ) , true ) ; 
		inputBlock.setText( defaultText ) ; 
		inputBlock.wrapTextX( 1500 ) ; 

		bTypingActive = false ; 
		maxChar = 24 ; 
		fontX = _fontX ; 
		fontY = _fontY ; 
		ofxAlphaStackItem::ofxAlphaStackItem() ; 
		ofxMatrixTransformObject::ofxMatrixTransformObject() ; 
		image.setAnchorPercent( 0.5 , 0.5f ) ; 
		x = _x ; 
		y = _y ; 
		rotation = 0.0f ; 
		scale = 1.0f ; 
		startText = "please type your email address" ; 
		inputBlock.setText( startText ) ; 
		text = "" ; 

		maxCharacters = 45 ; 
		endText = "Thanks for playing!" ; 
		reset() ; 
	}


	void loadValidateImage( string validatePath , float x , float y , ofPoint anchor ) 
	{
		validateImage.loadImage( validatePath , x , y , anchor , 0.0f ) ; 
	}

	void setupScoreBlock ( string fontLocation , int fontSize , ofColor _fontColor , int fontX , int fontY ) 
	{
		scoreBlock.init( fontLocation , fontSize , _fontColor , true ) ;
		scoreBlock.setProperties( fontX , fontY , OF_TEXT_ALIGN_CENTER ) ; 
		scoreBlock.wrapTextForceLines( 1 ) ; 

	}
	void draw ( ) 
	{
		validateImage.alphaStackUpdate( 1.0f ) ; 

		inputBlock.alpha = 1.0f ; 
		scoreBlock.alpha = 1.0f; 

		ofPushStyle( ) ; 
		ofSetColor( 255 , 255 ) ; // getOFAlpha() ) ; 
		ofPushMatrix() ; 
			ofTranslate( x , y ) ; 	
		//matrixPush() ; 
				image.draw( 0 , 0 ) ;
				if ( compositeFbo != NULL ) 
				{
					ofPushMatrix() ;
						ofTranslate( fboPosition.x , fboPosition.y ) ;
						ofScale( fboScale , fboScale ) ;
						ofSetColor( 255 , 255 ) ;
						compositeFbo->draw( 0 , 0  ) ; 
					ofPopMatrix(); 
				}
			
				inputBlock.drawLeft( fontX , fontY ) ; 


				ofSetColor( 255 ) ; 
				scoreBlock.draw( ) ; 


//				cout << " SCORE BLOCK - TEXT " << scoreBlock.rawText << " SCORE BLOCK ALPHA : " << scoreBlock.alpha << " SCORE BLOCK X ? Y " << scoreBlock.x << " , " << scoreBlock.y << endl ; 

				if ( validateImage.image.bAllocated() == true ) 
					validateImage.draw() ;
			
			//matrixPop(); 
		ofPopMatrix(); 

		ofPopStyle( ) ; 
	}

	
	void reset ( ) 
	{
		text = "" ; 
		bTypingActive = false ;
		compositeFbo = NULL ; 
		inputBlock.setText( defaultText ) ;

		bShiftActive = false ; 
	}

	void keyPressed ( int key ) 
	{
		
		//Can't have commas if we are storing address in a CSV.
		if ( key == ',' ) 
		{
			return ; 
		}

		if ( key == 8 ) 
		{
			if ( text.size() >= 2 ) 
			{
				text.pop_back() ; 
				inputBlock.setText( text ) ; 
			}
			else		
			{
				inputBlock.setText( startText ) ; 
				text = "" ; 
			}
	
			if ( bCheckIfEmailValid() == false && validateImage.alpha > 0.0f ) 
			{
				//Was valid now is not
				Tweenzor::add( &validateImage.alpha , validateImage.alpha , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
			}
			return ; 
		}

		//ofLogVerbose( "KeyboardInputPanel::keyPressed " ) << " key : " << key << endl ;
		if ( bTypingActive == true && text.size() < maxCharacters ) 
		{
			switch( key ) 
			{
				case 13:
					storedText = inputBlock.rawText ; 
					ofNotifyEvent( SUBMIT_DATA , storedText ) ; 
					bTypingActive = false ; 
					inputBlock.setText ( endText ) ; 
					ofLogNotice( " SUBMITTING DATA" ) ; 
					return ;	
					break ; 
					
			}
			

			if ( bShiftActive == false ) 
			{
				text += key  ; 
			}
			else
			{
				switch ( key ) 
				{
					case '2':
						text += "@" ;
						break ;

					case '-':
						text += "_" ; 
						break; 
				}
			}

			inputBlock.setText( text ) ; 


			if ( bCheckIfEmailValid() == true && validateImage.alpha == 0.0f ) 
			{
				//Was valid now is not
				Tweenzor::add( &validateImage.alpha , validateImage.alpha , 1.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
			}
		}
		else
		{
			/// nothing...
		}
	}

	/*
	void keyPressed ( int key ) 
	{
		string s = "" ; 
		s += key ;
		ofLogNotice( "KeyboardInputPanel::keyPressed " ) << " key : " << key << "char of : " << s << endl ;

		 if (key >=32 && key <=126) {
			

		 }
		 else
		 {
			 ofLogError( " NOT A VALID KEY " ) << key << endl ;
		 }
		/*
		if ( key >= 8 && key <= 122 ) 
		{
			 
			ofLogVerbose( s ) << " is a valid key" << endl ; 
			//nothing...
		}
		else
		{
			
			ofLogError( s ) << " is NOT a valid key and it's keycode '" << key << "' is out of range" << endl ; 
		//	return ; 
		}

		//if ( key == 342 &&  bCheckIfEmailValid() == true ) 
		//{
			
		//}
		
			switch( key ) 
			{
				case 13:
					if ( bCheckIfEmailValid() == true ) 
					{
						storedText = inputBlock.rawText ; 
						inputBlock.setText( startText ) ; 
						bTypingActive = false ; 
						bCheckIfEmailValid() ; 
						ofNotifyEvent( SUBMIT_DATA , storedText ) ; 
						
					}

					break ; 
					return ; 
			}
			if ( key == 8 ) 
			{
				if ( text.size() >= 2 ) 
				{
					text.pop_back() ; 
					inputBlock.setText( text ) ; 
				}
				else
				{
					inputBlock.setText( startText ) ; 
					text = "" ; 
				}

				if ( bCheckIfEmailValid() == false && validateImage.alpha > 0.0f ) 
				{
					//Was valid now is not
					Tweenzor::add( &validateImage.alpha , validateImage.alpha , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
				}
				return ; 
			}


			if ( bTypingActive == true && text.size() < maxCharacters ) 
			{

				text += key  ; 
				inputBlock.setText( text ) ; 

			}
			
			if ( bCheckIfEmailValid() == true && validateImage.alpha == 0.0f ) 
			{
				//Was valid now is not
				Tweenzor::add( &validateImage.alpha , validateImage.alpha , 1.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
			}
	}*/


	void keyReleased ( int key ) 
	{

	}
	/*
	bool input( float x , float y ) 
	{

		if ( bTypingActive == true && validateImage.alpha == 1.0f )
		{
			ofPoint p = ofPoint ( x , y ) ; 
			ofPoint btnPos = ofPoint ( submitButton.x + x , submitButton.y + y ) ; 

			float dist = p.distance( btnPos ) ; 
			if ( dist < 150.0f ) 
			{
				ofLogVerbose( " distance of : " ) << dist << " NOW CLICKED" << endl; 
				Tweenzor::add( &submitHit.alpha , 1.0f , 0.0f , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
				return true ; 
			}
		}

		return false ;
	}*/


	bool bCheckIfEmailValid( ) 
	{

		int atIndex = text.find( "@" ) ;
		int dotIndex = text.find_last_of( "." ) ;

		if ( text.size() > 2 && atIndex > 1 && dotIndex > atIndex ) 
			return true ; 
		else
			return false ; 
	}
};