#pragma once

#include "ofMain.h"

class GradientQuad
{
public :
	GradientQuad( ) { } 
	~GradientQuad( ) { } 

	ofFbo fbo ; 
	ofRectangle area ; 
	ofFloatColor upperLeft , upperRight ,  lowerRight ,  lowerLeft ; 

	void setup( ofRectangle _area , ofFloatColor _upperLeft , ofFloatColor _upperRight , ofFloatColor _lowerRight , ofFloatColor _lowerLeft ) 
	{
		area = _area ; 
		upperLeft = _upperLeft ; 
		upperRight = _upperRight ;  
		lowerRight =  _lowerRight ; 
		lowerLeft = _lowerLeft ; 
		fbo.allocate( area.width , area.height , GL_RGBA32F_ARB ) ;
		fbo.begin() ; 
			ofClear( 0 , 0 , 0, 1 ) ; 
		fbo.end() ; 
	}

	void drawIntoFbo( ) 
	{
		fbo.begin() ; 
			ofClear( 0 , 0 , 0 , 1 ) ; 
			glBegin(GL_QUADS);
				//Upper Left Vertex
				glColor3f( upperLeft.r , upperLeft.g , upperLeft.b );
				glVertex3f( 0.0f , 0.f , 0.0f );
			
				//Upper Right Vertex
				glColor3f( upperRight.r , upperRight.g , upperRight.b );
				glVertex3f( area.getWidth() , 0.0f , 0.0f );

				//Lower Right Vertex
				glColor3f( lowerRight.r , lowerRight.g , lowerRight.b );
				glVertex3f( area.getWidth() , area.getHeight() , 0.0f );

				//Lower Left Vertex
				glColor3f( lowerLeft.r , lowerLeft.g , lowerLeft.b );
				glVertex3f( 0.f , area.getHeight() , 0.0f );
			glEnd();
		fbo.end() ; 

	}

	void draw( bool bDrawIntoFbo = true ) 
	{
		//http://forum.openframeworks.cc/t/background-gradient/1304/2
		// draw a quad using the area and pass in colors
		// calculate the

		drawIntoFbo( ) ; 

		ofSetColor( 255 ) ; 
		fbo.draw( area.x , area.y ) ; 
	}
};