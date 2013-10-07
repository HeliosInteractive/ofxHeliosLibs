#pragma once
#include <stdint.h>
#include <Poco/Thread.h>
#include <ofTypes.h>
#include <ofThread.h>
#include <ofLog.h>
#include <ofUtils.h>

class ofxAtomicLog {
	static ofMutex _mutex;

public:
	static uint8_t _verBuff[sizeof (ofLogVerbose)];
	static uint8_t _notBuff[sizeof (ofLogNotice)];
	static uint8_t _warBuff[sizeof (ofLogWarning)];
	static uint8_t _errBuff[sizeof (ofLogError)];
	static uint8_t _fatBuff[sizeof (ofLogFatalError)];

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
#define ofxLogVer(x) if (ofGetLogLevel() > OF_LOG_VERBOSE) ; else ofxAtomicBlock((static_cast<ofLogVerbose *>(&(*new(ofxAtomicLog::_verBuff) ofLogVerbose() << "[" << ofxAtomicLog::threadId() << "] " << x)))->~ofLogVerbose())
#define ofxLogNot(x) if (ofGetLogLevel() > OF_LOG_NOTICE) ; else ofxAtomicBlock((static_cast<ofLogNotice *>(&(*new(ofxAtomicLog::_notBuff) ofLogNotice() << "[" << ofxAtomicLog::threadId() << "] " << x)))->~ofLogNotice())
#define ofxLogWar(x) if (ofGetLogLevel() > OF_LOG_WARNING) ; else ofxAtomicBlock((static_cast<ofLogWarning *>(&(*new(ofxAtomicLog::_warBuff) ofLogWarning() << "[" << ofxAtomicLog::threadId() << "] " << x)))->~ofLogWarning())
#define ofxLogErr(x) if (ofGetLogLevel() > OF_LOG_ERROR) ; else ofxAtomicBlock((static_cast<ofLogError *>(&(*new(ofxAtomicLog::_errBuff) ofLogError() << "[" << ofxAtomicLog::threadId() << "] " << x)))->~ofLogError())
#define ofxLogFat(x) if (ofGetLogLevel() > OF_LOG_FATAL_ERROR) ; else ofxAtomicBlock((static_cast<ofLogFatalError *>(&(*new(ofxAtomicLog::_fatBuff) ofLogFatalError() << "[" << ofxAtomicLog::threadId() << "] " << x)))->~ofLogFatalError())
