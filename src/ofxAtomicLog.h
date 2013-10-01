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

extern ofMutex _ofxAtomicLogMutex;

#define ofxAtomicBlock(x) { ofxAtomicLog::lock(); x; ofxAtomicLog::unlock(); }
#define ofxLogVer(x) { ofxAtomicBlock(ofLogVerbose() << "[" << ofxAtomicLog::threadId() << "] " << x); }
#define ofxLogNot(x) { ofxAtomicBlock(ofLogNotice() << "[" << ofxAtomicLog::threadId() << "] " << x); }
#define ofxLogWar(x) { ofxAtomicBlock(ofLogWarning() << "[" << ofxAtomicLog::threadId() << "] " << x); }
#define ofxLogErr(x) { ofxAtomicBlock(ofLogError() << "[" << ofxAtomicLog::threadId() << "] " << x); }
#define ofxLogFat(x) { ofxAtomicBlock(ofLogFatalError() << "[" << ofxAtomicLog::threadId() << "] " << x); }
