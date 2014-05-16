#ifndef TIMELINEMARKER_H
#define TIMELINEMARKER_H


class TimelineMarker
{
    public:
        TimelineMarker() {}
        virtual ~TimelineMarker() {}

        int index ;
        float videoPerc ;
        float stopVideoPerc ;
        ofColor color ;
        ofRectangle bounds ;

        void setup ( int _index , float _videoPerc , float _stopVideoPerc )
        {
            index = _index ;
            videoPerc = _videoPerc ;
            stopVideoPerc = _stopVideoPerc ;
            color = ofColor( ofRandom ( 255 ) , ofRandom( 255 ) , ofRandom( 255 )  ) ;
            bounds = ofRectangle( 0 , 0 , 25 , 100 ) ;
        }

        void update ( )
        {

        }

        void draw ( )
        {
            ofSetColor( color ) ;
            ofRect( bounds ) ;
        }

    protected:
    private:
};

#endif // TIMELINEMARKER_H
