#pragma once
#include <ofTypes.h>
#include "ofxAtomicLog.h"

class ofxScopeMutex {
	ofMutex &_mutex;

public:
	ofxScopeMutex(ofMutex &mutex): _mutex(mutex) {
		ofxLogVer("Locking mutex");
		_mutex.lock();
	}

	~ofxScopeMutex() {
		ofxLogVer("Unlocking mutex");
		_mutex.unlock();
	}
};
