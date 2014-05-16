#include "TextImageContent.h"
#include "ofxTweenzor.h"


void TextImageContent::setup ( string _path , ofRectangle _bounds )
{
    path = _path ;
    bounds = _bounds ;
    image.loadImage( path ) ;
    scrollBounds = ofRectangle ( 0 , -405 , image.width , 0 ) ;

    ofEnableAlphaBlending();

	mask.loadImage("image/debug_textImageMask.png");
	bounds.width = mask.width ;
    bounds.height = mask.height  ;
    firstInput = false ;
    dragSpeed = 0.0f ;

    cout << "image.height : " << image.height << endl ;

    //mask.loadImage( "shaders/mask.png") ;

	//set the texture parameters for the maks shader. just do this at the beginning
	maskShader.load("shaders/composite");
	maskShader.begin();
	maskShader.setUniformTexture("Tex0", image.getTextureReference(), 0);
	maskShader.setUniformTexture("Tex1", mask.getTextureReference(), 1);
	maskShader.end() ;
}

void TextImageContent::update()
{
    if ( firstInput == true )
        return ;

    if ( abs(dragSpeed) > 0.15f )
    {
        scroll.y += dragSpeed ;
        dragSpeed *= .94f ;
    }

    if ( scroll.y > scrollBounds.height )
    {
        dragSpeed = 0 ;
        //Bounce back ease
        Tweenzor::add( &scroll.y , scroll.y , scrollBounds.height , 0.0f , 0.2f , EASE_OUT_BACK ) ;
        //scroll.y = scrollBounds.height ;
    }

    if ( scroll.y < scrollBounds.y )
    {
        dragSpeed = 0 ;
        Tweenzor::add( &scroll.y , scroll.y , scrollBounds.y , 0.0f , 0.2f , EASE_OUT_BACK ) ;
        //scroll.y = scrollBounds.y ;
    }

}


void TextImageContent::inputBegin( ofVec2f input )
{
    //cout << "input begin! scroll" << scroll.y << endl ;
    Tweenzor::removeTween( &scroll.y ) ;
    firstInput = true ;
    startDrag = input ;
    startScroll = scroll.y ;
}

void TextImageContent::inputUpdate ( ofVec2f input )
{
    //cout << "input update!" << endl ;
   // updateDrag = input ;
    scroll.y = startScroll + ( input.y - startDrag.y ) ;
    updateDrag = input - lastInput ;
    lastInput = input ;
}

void TextImageContent::inputEnd( ofVec2f input )
{
    firstInput = false ;
    dragSpeed = updateDrag.y ;

    //cout << "input end! scroll" << scroll.y <<  endl ;
    //cout << "inputDrag: " << updateDrag << " | inputEnd: " << input << endl ;
    //cout << "dragSpeed is : " << dragSpeed << endl ;
}

void TextImageContent::draw ( )
{
    ofSetColor ( 255 , 255 , 255 ) ;

    maskShader.begin();

	//our shader uses two textures, the top layer and the alpha
	//we can load two textures into a shader using the multi texture coordinate extensions
	glActiveTexture(GL_TEXTURE0_ARB);
	image.getTextureReference().bind();

	glActiveTexture(GL_TEXTURE1_ARB);
	mask.getTextureReference().bind();

	//draw a quad the size of the frame
	glBegin(GL_QUADS);
	ofFill() ;
	ofSetColor ( 255 , 255 , 255 , 255 ) ;

	float maskOffset = -scroll.y ; //15 - mouseY;	/*
	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, maskOffset);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);
	glVertex2f( bounds.x, bounds.y );

	glMultiTexCoord2d(GL_TEXTURE0_ARB, image.getWidth(), maskOffset);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), 0);
	glVertex2f( bounds.x + bounds.width , bounds.y );

	glMultiTexCoord2d(GL_TEXTURE0_ARB, image.getWidth(), mask.getHeight() + maskOffset );
	glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), mask.getHeight() );
	glVertex2f( bounds.x + bounds.width , bounds.y + bounds.height );

	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, mask.getHeight() + maskOffset );
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, mask.getHeight() );
	glVertex2f( bounds.x , bounds.y + bounds.height );

	glEnd() ;

	//deactive and clean up
	glActiveTexture(GL_TEXTURE1_ARB);
	mask.getTextureReference().unbind();

	glActiveTexture(GL_TEXTURE0_ARB);
	image.getTextureReference().unbind();

    maskShader.end();

    //draw scrollbar indicator
    ofEnableAlphaBlending() ;
    ofSetColor ( 15 , 15 , 15 , 200 ) ;
    roundedRect ( bounds.x + bounds.width + 5 , bounds.y + (-scroll.y *.85) , 4 , 40 , 5 ) ;
}

void TextImageContent::roundedRect(float x, float y, float w, float h, float r )
{
    ofBeginShape() ;
    ofVertex( x+r , y ) ;
    ofVertex( x+w-r , y ) ;
    quadraticBezierVertex( x+w , y , x+w , y+r, x+w-r , y ) ;
    ofVertex( x+w , y+h-r ) ;
    quadraticBezierVertex( x+w , y+h , x+w-r , y+h , x+w, y+h-r ) ;
    ofVertex( x+r , y+h ) ;
    quadraticBezierVertex( x , y+h , x , y+h-r , x+r , y+h ) ;
    ofVertex( x , y+r ) ;
    quadraticBezierVertex( x , y , x+r , y , x , y+r ) ;
    ofEndShape() ;
}

void TextImageContent::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY )
{
    float cp1x = prevX + 2.0/3.0*(cpx - prevX) ;
    float cp1y = prevY + 2.0/3.0*(cpy - prevY) ;
    float cp2x = cp1x + ( x-prevX )/3.0 ;
    float cp2y = cp1y + ( y-prevY )/3.0 ;

    ofBezierVertex( cp1x , cp1y , cp2x , cp2y , x , y ) ;
}
