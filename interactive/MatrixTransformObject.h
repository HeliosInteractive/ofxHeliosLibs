#pragma once

class MatrixTransformObject
{
	public :
		MatrixTransformObject ( )
		{
			rotation = 0 ;
			scale = 1.0f ;
			x = 0 ;
			y = 0 ;
		}

		float rotation ;	// z rotation
		float scale ;		// uniform scale
		float x , y ;		// catersian points
    
		ofRectangle hitBounds ;

		void matrixPush( )
		{
			//translate -> rotate -> scale
			ofPushMatrix ( ) ;
			ofTranslate ( x , y , 0 ) ;
			ofScale ( scale , scale , 1.0f );
			ofRotateZ ( rotation ) ;

		}

		void matrixPop ( )
		{
			ofPopMatrix( ) ;
		}

		virtual void setHitBounds ( ofRectangle _rect )
		{
		    hitBounds = _rect ;
		}

		virtual ofRectangle getHitBounds ( )
		{
		    return hitBounds ;
		}

        virtual bool hitTest( ofVec2f p , ofRectangle _bounds )
        {
            return (( p.x > _bounds.x ) && ( p.x < _bounds.x + _bounds.width) && ( p.y > _bounds.y ) && ( p.y  < _bounds.y + _bounds.height ));
        }
};
