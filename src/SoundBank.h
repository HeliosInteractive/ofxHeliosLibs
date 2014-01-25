#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "ofMain.h"

class SoundBank
{
    public:
        SoundBank()
		{
			lastRandomIndex = -1 ;
			volume = 1.0f ; 
		}

        virtual ~SoundBank() {}

        vector<ofSoundPlayer * > sounds ;
		float volume ; 

        void addSound ( string path , float volume = 1.0f )
        {
           int i = sounds.size() ; 
           sounds.push_back( new ofSoundPlayer() ) ;
		   sounds [ i ]->loadSound( path ) ;
		   sounds [ i ]->setMultiPlay( false ); 
        }

		void loadFolder( string folderPath , float _volume = 1.0f  ) 
		{
			volume = _volume ; 
			ofDirectory dir  = ofDirectory( folderPath ) ; 
			
			dir.allowExt( "wav" ); 
			dir.allowExt( "aif" ) ;
			dir.allowExt( "mp3" ) ; 
			dir.allowExt( "aiff" ) ; 
			dir.allowExt( "m4a" ) ; 
			dir.listDir( ); 

			if ( dir.size() == 0 ) 
			{
				ofLogError( folderPath ) << " is empty ! Not loading any sounds" << endl ; 
				return ; 
			}

			for ( int i = 0 ; i < dir.size() ; i++ )
			{
				sounds.push_back( new ofSoundPlayer() ) ;
				sounds [ i ]->loadSound( dir.getPath( i )  ) ;
				sounds [ i ]->setMultiPlay( false ); 
				sounds [ i ]->setVolume( volume )  ; 
			}
		}

        int lastRandomIndex ;

        void playRandomSound ( )
        {

			
			if ( sounds.size() == 0 ) 
			{
				ofLogError( "SoundBank::playRandomSound()" ) << " is empty ! Not loading any sounds" << endl ; 
				return ; 
			}


			for( int i = 0 ; i < sounds.size() ; i++ ) 
			{
				
				if ( sounds[i]->getIsPlaying() == true ) 
					return ; 
			}
            //We wouldn't want to play the same random sound 2x in a row
            int randomIndex = lastRandomIndex ;
			if ( sounds.size() > 1 ) 
			{
				  while ( randomIndex == lastRandomIndex )
				 {
					   randomIndex = floor( ofRandom( ( sounds.size()-1 ) ) )  ;
				 }
			}

            lastRandomIndex = randomIndex ;
            ofLogVerbose( "SoundBank::playRandomSound" )  << " random index was : " << randomIndex << " of : " << (sounds.size()-1) << endl ;
			sounds[ randomIndex ]->setVolume( volume ) ; 
            sounds[ randomIndex ]->play() ;
			

        }
    protected:
    private:
};

#endif // SOUNDBANK_H
