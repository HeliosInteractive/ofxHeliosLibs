/*

 Image Thumbnail gallery created by Ben McChesney @ Helios Interactive
 similar to Image Gallery Content, but there are no forward / back buttons,
 only thumbnails to the side for navigation.

 */

#pragma once

#include <BaseContent.h>
#include "ImagePlus.h"

class ImageThumbnailGallery : public BaseContent
{
public:
    ImageThumbnailGallery() { } ;
        virtual ~ImageThumbnailGallery() { } ;

    void update() ;
    void inputUpdate ( ofVec2f input ) ;
    void inputBegin( ofVec2f input ) ;
    void inputEnd( ofVec2f input ) ;

    void setup ( string _path , ofRectangle _bounds ) ;
    void draw ( ) ;
    void drawMap ( ) ;

protected:
    ofDirectory dir ;
    vector<ImagePlus> gallery ;
    ofVec2f lastInput ;
    int focusGallery ;

private:
};
