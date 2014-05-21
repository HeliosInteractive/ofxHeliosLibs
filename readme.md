# ofxHeliosLibs

Set of libraries developed for Open Frameworks by Helios Interactive Technologies, Inc. Originally created for projects, we now abstracted them to be more flexible.

Groups 
================

* /content/ is all legacy code for working with certain content types, mostly still exists for backwards compatibility for legacy projects

* /ofxAlphaStack/ is used to calculate transparency on nested of combined objects, similar to Flash Movie Clips.

* /interactive/ contains a wide array of classes including : 
	
	DragScroller - make elastic scrolling functionality

	ofxMatrixTransformObject - an object with a screen position, scale, rotation

	ofxMatrixTransformImage - a versatile image with ofxAlphaStackItem + ofxMatrixTransformObject

	ofxImageButton - an easy to setup button with hit states

	VideoPlayer - using threads to scrub frames during seeking for performance
	
* /mediaBanks/

	SoundBank - load and play from a predetermined group of sounds

	VideoBank - load, play, and draw a predetermined group of videos

* /gesture/ - a few utilities and UI classes for implemented gesture cursors + interactions

* /pages/ - simple framework for linear experience that mirror webpage like flows

* /multitouch/ - utilities for developing multitouch gesture content

* /utils/ - general utilities for open Frameworks

* /text/ - collection of classes for Text and Font Rendering in OF. Currently implementing ofxFontStash

* /threaded_utils/ - collection of utilities for loading / exporting files via a thread.

* /ofxSync/ downloads content from a given set of URLs to a local directory. See [ofxSync.md](src/ofxSync/ofxSync.md) for more info.

* /ofxAtomicLog/thread safe logger developer as a part of ofxSync

* /loading/ legacy code for managing data being loaded over ofLoadUrlAsync() 
