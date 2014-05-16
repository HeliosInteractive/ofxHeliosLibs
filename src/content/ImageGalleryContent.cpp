#include "ImageGalleryContent.h"

ImageGalleryContent::ImageGalleryContent()
{
    //ctor

}

ImageGalleryContent::~ImageGalleryContent()
{
    //dtor
}

void ImageGalleryContent::drawUI ( )
{
    ofEnableAlphaBlending() ;
    ofSetColor( 255 , 255 , 255 , alpha * 255.0f ) ;
   // next.alpha = alpha/255.0f ;
   // prev.alpha = alpha/255.0f ;
    next.draw( ) ;
    prev.draw( ) ;
}

void ImageGalleryContent::setup ( string _path , ofRectangle _bounds )
{
    //cout << "ImageGallery : PixelHitTest == " << pixelHub << endl ;
    path = _path ;
    bounds = _bounds ;
    int numSlideshow = dir.listDir(path);
	dir.sort() ;

    //cout << " numSlideshow" << numSlideshow << " | " << dir.size() << endl ;

	for ( int i = 0 ; i < numSlideshow ; i++ )
	{
	    ImagePlus ip = ImagePlus() ;
	    ip.init(  dir.getPath( i ) , ofVec2f ( 0 , 0 ) , 0 , 0.5f , 0.5f ) ;
	    ip.x = -ip.image.width / 2 + ( i * ip.image.width ) ;
	    ip.y = -ip.image.height / 2 ;
	    gallery.push_back( ip ) ;
	}

	//bounds.x = 125 ;
	//bounds.y = gallery[0].image.height * -.7f ;

	initialValues() ;
	maxScale = 1.0f ;
}

void ImageGalleryContent::setupArrows ( string nextPath , string prevPath , string nextSelectedPath , string prevSelectedPath )
{
    next = ImagePlus (  ) ;
    prev = ImagePlus ( ) ;
    nextSelected = ImagePlus ( ) ;
    prevSelected = ImagePlus ( ) ;
	
    next.init ( nextPath , ofVec2f ( 150  ,  0 )  , 0 , 0.5, 0.5 ) ;
    prev.init ( prevPath , ofVec2f ( -150  , 0 ) , 0 , 0.5 , 0.5 ) ;
    nextSelected.init ( nextSelectedPath , ofVec2f ( 150  , 0 )  , 0 , 0.5, 0.5 );
    prevSelected.init ( prevSelectedPath , ofVec2f ( -150  , 0 ) , 0 , 0.5 , 0.5 ) ;
	
    nextSelected.alpha = 0.0f ;
    prevSelected.alpha = 0.0f ;


    //ofRectangle nextBounds = ofRectangle( )

    float halfSize = next.image.width ; /// 2.0f;
    ofRectangle nextRect = ofRectangle( next.x +- halfSize  * 2.0f, next.y +- halfSize  * 2.0f , halfSize * 2.0f , halfSize * 2.0f ) ;
    ofRectangle prevRect = ofRectangle( prev.x +- halfSize  * 2.0f , prev.y +- halfSize  * 2.0f , halfSize * 2.0f , halfSize * 2.0f ) ;
    next.setHitBounds ( nextRect ) ;
    prev.setHitBounds ( prevRect ) ;
    prev.inheritAlpha = true ;
    next.inheritAlpha = true ;

    initialValues() ;

    //n//ext.alpha = 125.0f ;
}

void ImageGalleryContent::setupPagination( string fillPath, string noFillPath )
{
    fillIndicator.loadImage( fillPath ) ; //ImagePlus ( fillPath , ofVec2f( 0 , 0 ) , 0.5 , 0.5 ) ;
    fillIndicator.setAnchorPercent( 0.5, 0.5 ) ;
    noFillIndicator.loadImage( noFillPath ) ; // = //ImagePlus ( noFillPath , ofVec2f ( 0 , 0 ) , 0.5, 0.5 ) ;
    noFillIndicator.setAnchorPercent( 0.5, 0.5 ) ;
}

void ImageGalleryContent::transitionOut( float time , float delay )
{
    Tweenzor::add( &alpha , alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
      Tweenzor::add( &scale , scale , 0.1f , delay , time , EASE_OUT_QUAD ) ;
    //Tweenzor::addCompleteListener( Tweenzor::getTween(&alpha) , this, &ImageGalleryContent::transitionComplete ) ;
    Tweenzor::addCompleteListener( Tweenzor::getTween(&alpha) , this , &ImageGalleryContent::transitionComplete ) ;

    gallery[focusGallery].alpha = 0.0f ;
    Tweenzor::add( &gallery[focusGallery].alpha , gallery[focusGallery].alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
//    Tweenzor::addCompleteListener( & alpha , this , ImageGalleryContent::)

    //Tweenzor::add( &next.alpha , next.alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    //Tweenzor::add( &prev.alpha , prev.alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    /*
    for ( int i = 0 ; i < gallery.size() ; i++ )
    {
        Tweenzor::add( &gallery[i].alpha , gallery[i].alpha , 0.0f , delay , time , EASE_OUT_QUAD ) ;
    }*/

    cout << "focusGalllery:" << focusGallery << endl ;
    gallery[focusGallery].inheritAlpha = true ;
}

void ImageGalleryContent::transitionComplete( float * args )
{
    inTransition = false ;
    if ( alpha == 0.0f )
    {
        gallery[focusGallery].alpha = 0.0f ;
    }

    gallery[focusGallery].inheritAlpha = false ;

    cout << "gallery  complete! inTransition : " << inTransition << endl ;

}

void ImageGalleryContent::transitionIn( float time , float delay )
{
    initialValues() ;

    Tweenzor::add( &scale , scale , maxScale , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::add( &alpha , 0.0f , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    gallery[focusGallery].inheritAlpha = true ;
    Tweenzor::add( &gallery[focusGallery].alpha , gallery[focusGallery].alpha , 1.0f , delay , time , EASE_OUT_QUAD ) ;
    Tweenzor::addCompleteListener( Tweenzor::getTween(&gallery[focusGallery].alpha) , this, &ImageGalleryContent::transitionComplete ) ;
    inTransition = true ;
}

void ImageGalleryContent::initialValues()
{
    galleryXOffset = 0.0f ;
    inTransition = false ;
    focusGallery = 0 ;
    alpha = 0.0f ;
    scale = 0.1f ;
    prev.alpha = 125.0f ;
    next.alpha = 255.0f ;
    for ( int a = 0 ; a < gallery.size() ; a++ )
	{
	    if ( a != 0 )
        {
            gallery[a].alpha = 0 ;
            gallery[a].inheritAlpha = false ;

        }
        else
        {
            gallery[a].alpha = 1.0f ;
            gallery[a].inheritAlpha = true ;
        }
	}
}

void ImageGalleryContent::update()
{

}

void ImageGalleryContent::inputUpdate ( ofVec2f input )
{

}

//Normalize the input vars being passed in
void ImageGalleryContent::inputBegin( ofVec2f input )
{
    if ( inTransition == true ) return ;

    int lastIndex = focusGallery ;

    float carouselTime = 0.35f ;

//    if ( hitTest( input , next.getHitBounds() ) == true )
 //   {
    if ( input.x > 0.5f )
    {
        //if ( next.inputTimeDelay < 0 )
        //    next.lastInputTime = ofGetElapsedTimef() ;

        if ( next.timeInputThreshold() == true )
        {
            if ( focusGallery < gallery.size() -1  )
                focusGallery += 1 ;
            else
            {
                Tweenzor::add( &next.alpha , next.alpha , 125.0f , 0.0f , 0.45f, EASE_OUT_QUAD ) ;
                  return ;
            }

            Tweenzor::add( &galleryXOffset , galleryXOffset , -focusGallery * gallery[0].image.width  , 0.0f , carouselTime , EASE_OUT_QUAD  ) ;
            nextSelected.alpha = 1.0f ;
            Tweenzor::add( &nextSelected.alpha , nextSelected.alpha , 0.0f , 0.2 , 0.3f , EASE_OUT_QUAD ) ;
        }
    }
    //b = prev.getHitBounds() ;
    //cout << " prev : !" << b.x << "," << b.y << "," << b.width << "," << b.height << endl;
    //if ( hitTest( input , prev.getHitBounds() ) == true )
    else
    {
        if ( prev.timeInputThreshold() )
        {
            if ( focusGallery > 0 )
                focusGallery -= 1 ;
            else
            {
                 Tweenzor::add( &prev.alpha , prev.alpha , 125.0f , 0.0f , 0.45f, EASE_OUT_QUAD ) ;
                  return ;
            }

            // indexDiff = abs( lastIndex - focusGallery ) ;
            Tweenzor::add( &galleryXOffset , galleryXOffset , -focusGallery * gallery[0].image.width  , 0.0f , carouselTime , EASE_OUT_QUAD ) ;
            prevSelected.alpha = 1 ;
            Tweenzor::add( &prevSelected.alpha , prevSelected.alpha , 0.0f , 0.2 , 0.3f , EASE_OUT_QUAD ) ;
        }
    }

    if ( focusGallery != lastIndex )
    {
        Tweenzor::add( &gallery[focusGallery].alpha , gallery[focusGallery].alpha , 1.0f , 0.2f , 0.15f , EASE_OUT_QUAD ) ;
      //  Tweenzor::add( &gallery[focusGallery].scale , 0.75 , 1.0f , 0.0f , 0.15f , EASE_OUT_QUAD ) ;

        //out
        gallery[lastIndex].inheritAlpha = false ;
        gallery[lastIndex].alpha = 1.0f ;
        Tweenzor::add( &gallery[lastIndex].alpha , 1.0f, 0.0f , 0.0f , 0.04f , EASE_IN_QUAD ) ;

      //  Tweenzor::add( &gallery[lastIndex].scale , 1.0f, 0.75f , 0.0f , 0.15f , EASE_IN_QUAD ) ;
    }

}

void ImageGalleryContent::inputEnd( ofVec2f input )
{

}


void ImageGalleryContent::drawMap ( )
{

}

void ImageGalleryContent::draw ( )
{
    ofEnableAlphaBlending() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    if ( alpha == 0 ) return ;
   // mainContent.draw ( 0 , -110) ;

    ofPushMatrix() ;
        ofTranslate ( galleryXOffset , 0 , 0 ) ;
        ofScale ( scale , scale , 1.0f ) ;

        for ( int i = 0 ; i < gallery.size() ; i++ )
        {
            gallery[i].draw (  ) ;
        }
    ofPopMatrix() ;

    float spacing = noFillIndicator.width + 2 ;
    float totalWidth = spacing * gallery.size() ;
    float offset = 510 + -totalWidth/2 ;
    //Pagination Indicator
    ofFill() ;
    ofSetColor ( 255 , 255 , 255 , 255.0f * alpha ) ;
    ofPushMatrix() ;
        //float _w = bounds.width + ( spacing * gallery.size() ) / 4 ;
        ofTranslate(  offset , bounds.y + 260 , 0 ) ;

       // ofSetColor ( 45 , 45 , 45 , 255.0f * alpha ) ;
        for ( int a = 0 ; a < gallery.size() ; a++ )
        {
            if( a != focusGallery )
            {
             //   fillIndicator.draw ( a * spacing , 0  ) ;
            }
            else
            {
             //   noFillIndicator.draw ( a * spacing , 0 ) ;
            }
        }

    ofPopMatrix() ;

   drawUI() ;
  //  nextSelected.draw() ;
  //  prevSelected.draw() ;


   // ofRect ( prev.x , prev.y ,prev.getHitBounds().width , prev.getHitBounds().height ) ;
}


