#ifndef BASECONTENT_H
#define BASECONTENT_H

#include "ofMain.h"
#include "ofxTransitionItem.h"
#include "ofxMatrixTransformObject.h"
#include "ofxAlphaStackItem.h"

class BaseContent : public ofxTransitionItem , public ofxMatrixTransformObject , public ofxAlphaStackItem
{
    enum ContentType
    {
        STATIC_IMAGE = 0 ,
        VIDEO = 1 ,
        VIDEO_REVEAL = 2 ,
        VIDEO_TIMELINE = 3
    };

    public:
        BaseContent( ) { } ;

        virtual void setup( string _path , ofRectangle _bounds ) { cout << "virtual setup" << endl ; }

        virtual ofTexture getTextureReference( )
        {
            cout << "getTExtureReference returning empty texture! " << endl ;
            return ofTexture() ;
        }

        //Use these bools to make transitions more modular
        bool bScale ;
        bool bTranslate ;
        bool bAlpha ;

        bool bDebug ;
        virtual void setDebug ( bool _bDebug )
        {
            bDebug = _bDebug ;
        }

        void setTransitionParams( bool _scale , bool _translate , bool _alpha )
        {
            bScale = _scale ;
            bTranslate = _translate ;
            bAlpha = _alpha ;
        }

        virtual ~BaseContent() {}
        virtual void draw() { }
        virtual void drawUI() { }
        virtual float getXOffset ( ) { return 0 ; }

        //In case we ever need the path , mainly for debugging ?
        virtual string getPath() { return path ; }

        //Bounds getter and setter. x/y position width/height is... well you know
        virtual ofRectangle getBounds ( ) { return bounds ; }
        virtual void setBounds ( ofRectangle _bounds ) { bounds = _bounds ; }

        //input
        virtual void inputBegin( ofVec2f input ) { cout << "Base inputBegin" <<endl ;  } ;      //First Contact
        virtual void inputUpdate ( ofVec2f input ) {  cout << "Base inputUpdate" <<endl ; } ;    //Engagement
        virtual void inputEnd( ofVec2f input ) {  cout << "Base inputEnd" <<endl ; } ;           //Departure
        virtual void update ( ) { } ;
        virtual void setFirstInput ( bool _firstInput ) { firstInput = _firstInput ; }
        virtual bool getFirstInput ( ) { return firstInput ; }

        virtual void transitionOut( float time = 0.25f, float delay = 0.0f ) { cout << "abstract transition out!" << endl ; }  ;
        virtual void transitionIn( float time = 0.25f, float delay = 0.0f ) { cout << "abstract transition In Called!" << endl ; } ;

        virtual void initialValues () { cout << " abstract initial values !" << endl ;  } ;


        virtual bool hitTest( ofVec2f p )
        {
            return (( p.x > bounds.x ) && ( p.x < bounds.x + bounds.width ) && ( p.y > bounds.y ) && ( p.y < bounds.y + bounds.height ) ) ;
        }

        virtual bool hitTest( ofVec2f p , ofRectangle _bounds )
        {
            return (( p.x > _bounds.x ) && ( p.x < _bounds.x + _bounds.width) && ( p.y > _bounds.y ) && ( p.y  < _bounds.y + _bounds.height ));
        }

		ofPoint offset ; 
        ofPoint getOffset ( ) { return offset ; }
        void setOffset ( ofPoint _offset ) { offset = _offset ; }

        ofImage mainContent ;

        virtual int getWidth() { return 0 ; }
        virtual int getHeight() { return 0 ; }

        float maxScale ;

        void setType( int _t )
        {
            switch ( _t )
            {
                case 0 :
                    cType = STATIC_IMAGE ;
                    break ;

                case 1 :
                    cType = VIDEO ;
                    break ;

                case 2 :
                    cType = VIDEO_REVEAL ;
                    break ;

                case 3 :
                    cType = VIDEO_TIMELINE ;
                    break ;

                default :
                    cout << "type not found! " << endl ;
                    break ;
            }
        }

        virtual int getType( )
        {
             return cType ;
        }

    protected:
        string path ;
        ofRectangle bounds ;
        bool firstInput ;
        ContentType cType ;

    private:

};

#endif // BASECONTENT_H
