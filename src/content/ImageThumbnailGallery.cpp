//
//  ImageThumbnailGallery.cpp
//  MVW_CES
//
//  Created by Ben McChesney on 11/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ImageThumbnailGallery.h"

void ImageThumbnailGallery::setup ( string _path , ofRectangle _bounds )
{
    path = _path ;
    bounds = _bounds ;
    int numSlideshow = dir.listDir(path);
    numSlideshow = 4 ;
	dir.sort() ;

    int startX = 500 ;
    int startY = 125 ;
    int row = 0 ;
    int col = 0 ;
	for ( int i = 0 ; i < numSlideshow ; i++ )
	{
	    //
	    ImagePlus ip = ImagePlus() ;
	    ip.init ( dir.getPath( i ) , ofVec2f ( 0 , 0 )  ) ;
	    gallery.push_back( ip ) ;
       // gallery[i].position.x = ( gallery[i].image.width + 5 ) ;
	}

	for ( int a = 0 ; a < numSlideshow ; a++ )
	{
	    if ( a!= 0 )
            gallery[a].transitionOut() ;

        if ( a < 4 )
        {
            if ( a % 2 == 0 )
            {
                row++ ;
                col = 0 ;
            }

            gallery[a].setHitBounds ( ofRectangle( startX + col * 55 , startY + row * 55 , 50 , 50 )  ) ;

            col++ ;
        }
	}

	bounds.x = 125 ;
	bounds.y = gallery[0].image.height * -.7f ;

	focusGallery = 0 ;
}

void ImageThumbnailGallery::update()
{


}

void ImageThumbnailGallery::inputUpdate ( ofVec2f input )
{

}

/*
void ImageThumbnailGallery::inputFromHex ( int hex )
{

    int lastIndex = focusGallery ;
    //4 s the max # of slideshows
    for ( int i = 0 ; i < 4 ; i++ )
    {
        if ( hex == gallery[i].getMapHexColor() && i != focusGallery )
        {
            gallery[lastIndex].transitionOut() ;
            focusGallery = i ;
            gallery[focusGallery].transitionIn() ;
        }
    }
}
*/

void ImageThumbnailGallery::inputBegin( ofVec2f input )
{
    //cout << "input.x " << input.x << " | " << bounds.x << endl ;
    // float normalX = ( input.x - bounds.x ) / bounds.width  ;
    if ( input.x > 2 )
        return ;
    //cout << " width : " << bounds.width << " normalX : " << normalX << endl ;
    /*
    if ( focusGallery != lastIndex )
    {
        gallery[lastIndex].transitionOut() ;
        gallery[focusGallery].transitionIn() ;
    }*/

}

void ImageThumbnailGallery::inputEnd( ofVec2f input )
{

}


void ImageThumbnailGallery::drawMap ( )
{

/*
    ofPushMatrix() ;
    ofTranslate ( bounds.x , bounds.y , 0 ) ;
    //4 items on the side
    for ( int i = 0 ; i < 4 ; i++ )
    {
        ofSetHexColor (  gallery[i].getMapHexColor() ) ;
        ofRect ( gallery[i].getHitRect() ) ;
    }
    ofPopMatrix() ;

 //   cout << "drawing a map on ImageThumbnail gallery !" << endl ;
 */
}

void ImageThumbnailGallery::draw ( )
{
    ofPushMatrix() ;
    ofTranslate ( bounds.x , bounds.y , 0 ) ;

    for ( int i = 0 ; i < gallery.size() ; i++ )
    {
        gallery[i].draw (  ) ;
    }

    ofSetColor ( 255 , 255 , 255 ) ;
    for ( int a = 0 ; a < gallery.size() ; a++ )
    {
        ofRectangle r = gallery[a].getHitBounds() ;
        gallery[a].image.draw ( r.x , r.y , r.width , r.height ) ;
    }

    ofPopMatrix() ;
}


