#include "EasyMask.h"

EasyMask::EasyMask()
{
    //ctor
}

EasyMask::~EasyMask()
{
    //dtor
}

 void EasyMask::setup( string shaderPath , ofRectangle _area )
 {
     maskShader.load( shaderPath ) ; //"shaders/composite" ) ;
     maskArea = _area ;
     originalMaskArea = _area ;
 }

void EasyMask::setMask( string maskPath )
{
    mask.loadImage( maskPath ) ;
}
void EasyMask::setMask( ofImage newMask )
{
    mask = newMask ;
}

void EasyMask::drawMask ( ofTexture contentTex , ofTexture maskTex , float xOffset , float yOffset , float contentAlpha )
{
    /*
    if ( contentTex.getWidth() < maskArea.width )
        maskArea.width = contentTex.getWidth() ;
    if ( contentTex.getHeight() < maskArea.height )
        maskArea.height = contentTex.getHeight() ;
    */
    //TEX0 = CONTENT
    //TEX1 = M

    //BEGIN MASK
    ofEnableAlphaBlending( ) ;
	//ofSetColor ( 255 , 255 , 255 ) ;
      //  contentTex.setTextureWrap( GL_CLAMP, GL_CLAMP ) ;

		glActiveTexture(GL_TEXTURE0_ARB);
		contentTex.bind();

		glActiveTexture(GL_TEXTURE1_ARB);
		maskTex.bind();

            //prevents weird texture wrapping
           // contentTex.setTextureWrap( GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB ) ;
            contentTex.setTextureWrap( GL_CLAMP_TO_BORDER_ARB , GL_CLAMP_TO_BORDER_ARB ) ;
          //  texture1.setTextureWrap( GL_CLAMP , GL_CLAMP ) ;

            //xOffset = sin ( ofGetElapsedTimef() ) * 500.0f ;
            maskShader.begin();

                maskShader.setUniformTexture("Tex0", contentTex , 0);
                maskShader.setUniformTexture("Tex1", maskTex , 1);
                maskShader.setUniform1f( "alpha" , contentAlpha ) ;
                glBegin(GL_QUADS);
                ofFill() ;
              //  ofSetColor ( 255 , 255 , 255 , 255 ) ;

              //  maskOffset = 0 ; //sin ( ofGetElapsedTimef() ) * 200.0f ;
                glMultiTexCoord2d(GL_TEXTURE0_ARB, xOffset , yOffset );
                glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, contentAlpha );
                glVertex2f( maskArea.x ,  maskArea.y );


                glMultiTexCoord2d(GL_TEXTURE0_ARB, xOffset + contentTex.getWidth(), yOffset );
                glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTex.getWidth(), 0 );
                glVertex2f(  maskArea.x + maskArea.width , maskArea.y );


                glMultiTexCoord2d(GL_TEXTURE0_ARB, xOffset + contentTex.getWidth() , contentTex.getHeight() + yOffset );
                glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTex.getWidth() , maskTex.getHeight() );
                glVertex2f(  maskArea.x + maskArea.width  , maskArea.height + maskArea.y );

                glMultiTexCoord2d(GL_TEXTURE0_ARB, xOffset , contentTex.getHeight() + yOffset );
                glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskTex.getHeight()  );
                glVertex2f(  maskArea.x , maskArea.height + maskArea.y ) ;

                glEnd();

            maskShader.end() ;
		//deactive and clean up
		glActiveTexture(GL_TEXTURE1_ARB);
		maskTex.unbind();

		glActiveTexture(GL_TEXTURE0_ARB);
		contentTex.unbind();

		//maskArea = originalMaskArea ;
}
