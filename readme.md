# ofxHeliosLibs

Set of libraries developed for Open Frameworks by Helios Interactive Technologies, Inc. Originally created for projects, we now use them as the base for most UIs.

* AlphaStackItem is used to calculate transparency on nested of combined objects, similar to Flash Movie Clips. **example currently broken**

* TransitionItem is used to coordinate items transitioning in and out.

* MatrixTransformObject is used for 2D UIs to allow for scaling, positioning, and rotation, similar to Flash Movie Clips.

* SoundBank is used to play one of random sounds for a given action.

* MultiTouchActions is a hub for calculating the three main multitouch gestures (scale, rotate, translate) 

* MultiTouchCursor is a wrapper for normalized input data so we don't have to switch code between TUIO + Windows 7 for instance.

* MultiTouchObject is a basic example class for multitouch input data.

* ofxSync downloads content from a given set of URLs to a local directory. See [ofxSync.md](src/ofxSync.md) for more info.
