#include "ImageContent.h"
#include "Tweenzor.h"

void ImageContent::setup ( string _path , ofRectangle _bounds )
{
    path = _path ;
    setHitBounds( _bounds ) ;
    bool didLoad = image.loadImage( path ) ;
    image.setAnchorPercent( 0.5 , 0.5 ) ;
    alpha = 0.0f ;
    offset = ofPoint( ) ;
     maxScale = 1.0f ;
   // cout << "loaded path is : " << path << " :: " << didLoad << endl ;

}

void ImageContent::initialValues( )
{
    alpha = 0.0f ;
}


void ImageContent::draw ( )
{
    if ( alpha == 0 ) return ;

    ofEnableAlphaBlending() ;
    //ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    //mainContent.draw ( 0 , -108) ;
    //matrixPush () ;
    ofPushMatrix( ) ;
      //  cout << " x  " << x << " y : " << y << endl ;
        ofTranslate( offset.x , offset.y , 0 ) ;
        ofScale ( scale , scale ) ;
        image.draw ( 0 , 0 ) ;
    ofPopMatrix( ) ;
    //matrixPop () ;
    //image.draw ( x , y , image.width , image.height ) ;
}
void ImageContent::transitionIn( ofVec2f _offset , float time , float delay )
{
    Tweenzor::add( &scale , scale , maxScale , delay , time, EASE_OUT_QUAD ) ;
    Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &offset.x , offset.x , _offset.x , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &offset.y , offset.y , _offset.y , delay , time , EASE_OUT_QUAD ) ;

}
void ImageContent::transitionOut( ofVec2f _offset , float time , float delay )
{
    Tweenzor::add( &scale , scale , maxScale , delay , time, EASE_OUT_QUAD ) ;
    Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &offset.x , offset.x , _offset.x , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &offset.y , offset.y , _offset.y , delay , time , EASE_OUT_QUAD ) ;

}

void ImageContent::transitionOut( float time , float delay )
{
    Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
     Tweenzor::add( &scale , scale , 0.1f , delay , time, EASE_OUT_QUAD ) ;
}

void ImageContent::transitionIn ( float time , float delay )
{
    initialValues() ;

    Tweenzor::add( &scale , scale , maxScale , delay , time, EASE_OUT_QUAD ) ;
    Tweenzor::add( &alpha , alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
}

