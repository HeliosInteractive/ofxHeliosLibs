#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "ofMain.h"

class SoundBank
{
    public:
        SoundBank() {}
        virtual ~SoundBank() {}


        vector<ofSoundPlayer> sounds ;


        void addSound ( string path , bool bMultiPlay = false )
        {
            ofSoundPlayer s ;
            s.loadSound( path ) ;
            s.setMultiPlay( bMultiPlay ) ;
            sounds.push_back( s ) ;
            lastRandomIndex = -1 ;
        }

        int lastRandomIndex ;

        void playRandomSound ( )
        {
            //We wouldn't want to play the same random sound 2x in a row
            int randomIndex = lastRandomIndex ;
            while ( randomIndex == lastRandomIndex )
            {
                randomIndex = ofRandom( (sounds.size()) ) ;
            }

            lastRandomIndex = randomIndex ;
            cout << "random index was : " << randomIndex << " of : " << sounds.size() << endl ;
            sounds[ randomIndex ].play() ;


        }
    protected:
    private:
};

#endif // SOUNDBANK_H
