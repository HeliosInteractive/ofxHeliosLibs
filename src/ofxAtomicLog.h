#pragma once
#include <Poco/Thread.h>
#include <ofTypes.h>
#include <ofThread.h>
#include <ofLog.h>
#include <ofUtils.h>

class ofxAtomicLog {
	static ofMutex _mutex;
public:
	static void lock() { _mutex.lock(); }
	static void unlock() { _mutex.unlock(); }
	// std::setw() doesn't seem to work with logging and getCurrentThread()
	// seems to be broken; use POCO and our own formatting
	static const std::string threadId() {
		Poco::Thread *thread = Poco::Thread::current();
		return thread == 0 ? "   OF" : ofToString(thread->id(), 5, ' ');
	}
};

// a little messy. here's why:

// (1) what we're solving here: the destructor of the ofLogVerbose(),
// ofLogNotice(), ... objects does the logging, so we need to trigger
// the destructor of these objects before we unlock our mutex.

// (2) while we are at this, let's check the log level, before evaluating
// the log expression, so that we don't evaluate to build the log message,
// just to later throw the log message away, in case the log level prevents
// the message from being logged

// (3) use "if (...) ; else ..." so that we don't break any "if" branches
// in the code that uses our macros

#define ofxAtomicBlock(x) ofxAtomicLog::lock(), x, ofxAtomicLog::unlock()
#define ofxLogVer(x) if (ofGetLogLevel() > OF_LOG_VERBOSE) ; else ofxAtomicBlock(delete &(*new ofLogVerbose() << "[" << ofxAtomicLog::threadId() << "] " << x))
#define ofxLogNot(x) if (ofGetLogLevel() > OF_LOG_NOTICE) ; else ofxAtomicBlock(delete &(*new ofLogNotice() << "[" << ofxAtomicLog::threadId() << "] " << x))
#define ofxLogWar(x) if (ofGetLogLevel() > OF_LOG_WARNING) ; else ofxAtomicBlock(delete &(*new ofLogWarning() << "[" << ofxAtomicLog::threadId() << "] " << x))
#define ofxLogErr(x) if (ofGetLogLevel() > OF_LOG_ERROR) ; else ofxAtomicBlock(delete &(*new ofLogError() << "[" << ofxAtomicLog::threadId() << "] " << x))
#define ofxLogFat(x) if (ofGetLogLevel() > OF_LOG_FATAL_ERROR) ; else ofxAtomicBlock(delete &(*new ofLogFatalError() << "[" << ofxAtomicLog::threadId() << "] " << x))
