#include "ofxImageButton.h"

void ofxImageButton::setup ( ) 
{
	//bToggeled = true ; 
	bHit = false ; 
	tweenHitArgs = 0.0f ;

	sprite.setup( ) ; 
	hitSprite.setup( ) ; 
}

void ofxImageButton::update( ) 
{
	sprite.alphaStackUpdate( 1.0f ); 
	hitSprite.alphaStackUpdate( 1.0f ) ; 
}

void ofxImageButton::loadImage( string spritePath , float _x , float _y , ofPoint _anchor , float _alpha , string hitSpritePath ) 
{
	anchor = _anchor ; 
	sprite.loadImage( spritePath , _x , _y , anchor , _alpha ) ; 
	hitSprite.loadImage( hitSpritePath , _x , _y , anchor , 0.0f ) ; 
}

void ofxImageButton::setBoundsPadding( float ratio  ) 
{
	float w = hitSprite.image.getWidth() ; 
	float h = hitSprite.image.getHeight() ; 
	sprite.setHitBounds( ofRectangle( sprite.x - ( w * ratio  * anchor.x ) ,
									  sprite.y - ( h * ratio * anchor.y ) ,
									  hitSprite.image.getWidth() * ratio ,
									  hitSprite.image.getHeight() * ratio ) )  ; 
}


void ofxImageButton::draw( ) 
{
	ofPushMatrix() ; 
		sprite.draw( ) ; 
		hitSprite.draw( ) ; 
	ofPopMatrix() ; 
}


bool ofxImageButton::hitTestPoint ( ofPoint p ) 
{
	if ( bHit == false ) // && bActive == true ) 
	{
		bool bHitResult = sprite.getHitBounds( ).inside( p ) ;
		if ( bHitResult ) 
		{
			bHit = true ; 
			onHit( ) ; 
		}

		return bHitResult ; 
	}

	return false ;
}

void ofxImageButton::onReset( ) 
{
	Tweenzor::add( &sprite.scale , sprite.scale , 1.0 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &sprite.alpha , sprite.alpha , 1.0 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &hitSprite.alpha , hitSprite.alpha , 0.0f , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &hitSprite.scale , hitSprite.scale , 0.0f , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  

}

void ofxImageButton::onHit( ) 
{
	bHit = true ; 
	tweenHitArgs = 0.0f ; 
	Tweenzor::add( &sprite.scale , sprite.scale , 1.25 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &tweenHitArgs , 0.0f , 1.0f, 0.0f , 0.1f ) ; 
	Tweenzor::addCompleteListener( Tweenzor::getTween( &tweenHitArgs ) , this , &ofxImageButton::hitStageComplete ) ; 
}

void ofxImageButton::hitStageComplete( float * args ) 
{
	Tweenzor::add( &sprite.scale , sprite.scale , 0.1 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &sprite.alpha , sprite.alpha , 0.0 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &hitSprite.alpha , 0.0f , 1.0 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &hitSprite.scale , 0.1f , 1.0 , 0.0f , 0.1f , EASE_OUT_QUAD ) ;  

	Tweenzor::add( &tweenHitArgs , 0.0f , 1.0f, 0.0f , 0.1f ) ; 
	Tweenzor::addCompleteListener( Tweenzor::getTween( &tweenHitArgs ) , this , &ofxImageButton::hitComplete ) ; 
}

void ofxImageButton::hitComplete( float * args ) 
{
	bHit = false ; 
}