#include "ofxTextSuite.h"
#include "Tweenzor.h"

class ConsoleTextBlock
{
	public :
		ConsoleTextBlock( ) { }

		ofxTextBlock textBlock ; 
		float printLerp ; 
		bool bInTransition ;
		float textWrapX ;

		string text ; 
		string printedText ; 
			
		void init ( FontObject fo , int fontSize, string _text )
		{
			textBlock.init ( fo.path , fontSize ) ; 
			printLerp = 0.0f ;
			bInTransition = false ;
			textWrapX = 380.0f ; 

			text = _text ; 
			textBlock.setText( " " ) ;
		}

		void update ( ) 
		{
			if ( bInTransition == false ) 
				return ;

			int maxIndex = text.size() ;
			int stringLength = printLerp * maxIndex ; 
			printedText = text.substr( 0 , stringLength ) ; 
			textBlock.setText ( printedText ) ;
			textBlock.wrapTextX( textWrapX ) ; 

		}

		void transitionIn ( float time , float delay ) 
		{
			bInTransition = true ; 
			Tweenzor::add ( &printLerp , 0.0f , 1.0f , delay , time ) ; 
		}

		void transitionInComplete ( float * args ) 
		{
			bInTransition = false ;	
		}

		void transitionOut ( float time , float delay  )
		{
			bInTransition = true ; 
			Tweenzor::add ( &printLerp , 1.0f , 0.0f , delay , time ) ; 
		}

		void transitionOutComplete ( float * args ) 
		{
			bInTransition = false ; 
		}

		void draw ( float x , float y ) 
		{
			textBlock.draw( x , y ) ; 
		}
};