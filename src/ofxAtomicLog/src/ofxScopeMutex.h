// Copyright (c) 2013 Helios Interactive Technologies, Inc. (heliosinteractive.com)

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once
#include <ofTypes.h>
#include "ofxAtomicLog.h"

class ofxScopeMutex {
	ofMutex &_mutex;
	bool _quiet;

public:
	ofxScopeMutex(ofMutex &mutex, bool quiet = false): _mutex(mutex), _quiet(quiet) {
		if (!_quiet) {
			ofxLogVer("Locking mutex");
		}
		_mutex.lock();
	}

	~ofxScopeMutex() {
		if (!_quiet) {
			ofxLogVer("Unlocking mutex");
		}
		_mutex.unlock();
	}
};
