#include "ofxAtomicLog.h"

ofMutex ofxAtomicLog::_mutex;
uint8_t ofxAtomicLog::_verBuff[sizeof (ofLogVerbose)];
uint8_t ofxAtomicLog::_notBuff[sizeof (ofLogNotice)];
uint8_t ofxAtomicLog::_warBuff[sizeof (ofLogWarning)];
uint8_t ofxAtomicLog::_errBuff[sizeof (ofLogError)];
uint8_t ofxAtomicLog::_fatBuff[sizeof (ofLogFatalError)];
