
#include "RoundedGradientPanel.h"

void RoundedGradientPanel::setup( ofRectangle area, float _padding , float _cornerRadius  )  
{
	ofLogNotice() << "RoundedGradientPanel::setup - " << area ;
	padding = _padding ; 
	rect = area; 
	cornerRadius = _cornerRadius ; 
	
	ofColor darkBlue = ofColor( 20.f , 22.f , 32.f ) ; 
	ofColor darkBlueCopy = darkBlueCopy ; 
	ofColor blue = ofColor( 36.0f , 45.f , 66.f ) ; 
	ofColor mid = darkBlueCopy.lerp( blue , 0.5f ) ;
	//roundedRect.setup( blue , ofRectangle( 0 , 0 , area.getWidth() , area.getHeight() ) , false ) ; 
	quad.setup( ofRectangle( 0 , 0 , rect.getWidth() , rect.getHeight() ) , blue , darkBlue , darkBlue , blue )  ;
	
	ofxAlphaStackItem::setup() ; 

	//mask.setup( "shader/maskShader/composite" , rect ) ; 

	//maskFbo.allocate( rect.getWidth() , rect.getWidth() ) ; 
	//maskFbo.begin(); 
	//	ofClear( 0 , 0 , 0 , 1 ); 
	//maskFbo.end(); 
	
}

void RoundedGradientPanel::update() 
{
	//roundedRect.bounds = ofRectangle ( rect.x - padding , rect.y - padding, rect.width + padding * 2 , rect.height + padding * 2 )  ; 
	quad.area = rect ;   
	//roundedRect.alphaStackUpdate( alpha ) ; 
	mask.maskArea = rect ; //roundedRect.bounds ; 
	float w = rect.getWidth() ; 
	float h = rect.getHeight() ; 
	
	//ofSetLogLevel( OF_LOG_SILENT ) ; 


	if ( maskFbo.getWidth() < w || quad.fbo.getWidth() < w ) 
	{
		//cout << " MASK was : " << maskFbo.getWidth() << " x " << maskFbo.getHeight() << endl; 
		maskFbo.allocate( w , h ) ;
		quad.fbo.allocate( w , h ) ; 
		//cout << " is NOW : " << maskFbo.getWidth() << " x " << maskFbo.getHeight() << endl; 
	}
	else if ( maskFbo.getHeight() < h || quad.fbo.getHeight() < h ) 
	{
		//cout << " MASK was : " << maskFbo.getWidth() << " x " << maskFbo.getHeight() << endl; 
		maskFbo.allocate( w , h ) ; 
		quad.fbo.allocate( w , h ) ; 
		//cout << " is NOW : " << maskFbo.getWidth() << " x " << maskFbo.getHeight() << endl; 
	}
	

}

void RoundedGradientPanel::draw() 
{
	if ( alpha == 0 ) return ; 


	/*
	maskFbo.begin() ; 
		ofPushMatrix() ; 
			ofTranslate( -roundedRect.bounds.x , -roundedRect.bounds.y ) ; 
			roundedRect.draw() ; 
		ofPopMatrix() ;
	maskFbo.end() ; 
	quad.drawIntoFbo( ) ; 
	mask.drawMask( quad.fbo.getTextureReference() , maskFbo.getTextureReference() , 0 , 0, 255.0f , rect.getWidth() , rect.getHeight() ); 
	*/
	
	
	
	

	ofPushMatrix( ) ; 
		//ofTranslate( -roundedRect.getBounds().x , -roundedRect.getBounds().y ) ;
		ofColor blue = ofColor( 36.0f , 45.f , 66.f ) ; 
		ofSetColor( blue , getOFAlpha() ) ; 
		ofRectRounded( rect , cornerRadius ) ; 
	ofPopMatrix() ; 

	
	//quad.fbo.draw( 0 , 0 ) ; 
	//quad.fbo.draw( 0 , 0 , rect.getWidth() , rect.getHeight()  );
	//stringstream ss ; 
	//ss << "rect : " << rect << endl ; 
	//ss << "rounded rect area : " << roundedRect.bounds << endl ; 
	//ss << "FBO dims : " << quad.fbo.getWidth() << " x " << quad.fbo.getHeight() << endl; 
	//ofDrawBitmapStringHighlight( ss.str() , 50 , 150 ) ; 
}

		

void RoundedGradientPanel::transitionIn( ) 
{

}

void RoundedGradientPanel::transitionOut() 
{

}