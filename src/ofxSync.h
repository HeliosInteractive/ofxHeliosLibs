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
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <string>
#include <list>
#include <algorithm>
#include <climits>
#include <ofMain.h>
#include <Poco/DigestEngine.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/DNS.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include "ofxAtomicLog.h"
#include "ofxScopeMutex.h"
#include "md5.h"

class ofxSync {
public:
	enum SyncStatus {
		SyncStatusStarted,
		SyncStatusProgress,
		SyncStatusFailure,
		SyncStatusComplete,
		SyncStatusInterrupted
	};

	typedef void (*SyncCallback)(void *opaque, SyncStatus status, int32_t id,
		const std::string &fileName, int64_t thisLength, int64_t thisTotal, int64_t allLeft,
		int64_t allTotal, int32_t countLeft, int32_t countTotal, int32_t attempt);

	class FileInfo {
		FileInfo &operator=(const FileInfo &rhs);

	public:
		std::string _url;
		std::string _fileName;
		int64_t _length;
		std::string _md5Digest;

		FileInfo(const std::string &url, const std::string &fileName = "", int64_t length = -1,
			const std::string &md5Digest = ""): _url(url), _fileName(fileName), _length(length),
			_md5Digest(md5Digest) {}
		FileInfo(const FileInfo &other): _url(other._url), _fileName(other._fileName),
			_length(other._length), _md5Digest(other._md5Digest) {}
		std::string toString() const;
	};

	typedef std::map<std::string, FileInfo> FileMap;
	typedef std::pair<std::string, FileInfo> FileMapElement;
	typedef std::list<FileInfo> FileList;

private:
	class SyncRecord: public FileInfo {
		SyncRecord &operator=(const SyncRecord &rhs);

	public:
		int32_t _threadId;
		int32_t _attempt;
		bool _resume;
		std::string _filePath;
		std::string _infoPath;
		std::string _dataPath;
		std::string _hashPath;
		bool _lenient;
		int64_t _received;

		SyncRecord(const FileInfo &info, bool lenient): FileInfo(info), _threadId(-1), _attempt(0),
			_resume(true), _lenient(lenient), _received(0) {}
		SyncRecord(const SyncRecord &other): FileInfo(other), _threadId(other._threadId),
			_attempt(other._attempt), _resume(other._resume), _filePath(other._filePath),
			_infoPath(other._infoPath), _dataPath(other._dataPath), _hashPath(other._hashPath),
			_lenient(other._lenient), _received(other._received) {}
		SyncRecord(const std::string &fileName, const std::string &filePath, int64_t length):
			FileInfo("", fileName, length, ""), _threadId(-1), _attempt(0), _resume(true),
			_filePath(filePath), _lenient(false), _received(0)
			{}
		SyncRecord(): FileInfo("", "", -1, ""), _threadId(-1), _attempt(0), _resume(true),
			_lenient(false), _received(0) {}
		bool writeToFile(const std::string &path);
		bool readFromFile(const std::string &path);
		std::string toString() const;
	};

	class Md5Context: public MD5_CTX {
	public:
		bool writeToFile(const std::string &path);
		bool readFromFile(const std::string &path);
		static std::string resultToString(const uint8_t md5Result[16]);
	};

	class SyncThread: public ofThread {
		static const int32_t BUFFER_SIZE = 10000;
		static const std::string BOB;

	public:
		enum SyncThreadState {
			SyncThreadStateClaim,
			SyncThreadStateConnect,
			SyncThreadStateDownload,
			SyncThreadStatePersist,
			SyncThreadStateError,
			SyncThreadStateComplete,
			SyncThreadStateExit
		};

		int32_t _threadId;

	private:
		ofxSync *_sync;
		SyncThreadState _state;
		SyncRecord *_record;
		int64_t _received;
		Md5Context _md5Context;
		ofFile *_dataFile;
		Poco::Net::HTTPClientSession *_session;
		std::istream *_stream;
		char _buffer[BUFFER_SIZE];

		void runCallback(SyncStatus status, bool lock);
		bool stateClaim(bool &resume);
		bool stateConnect(bool &resume);
		bool stateDownload(bool &resume);
		bool statePersist(bool &resume);
		void stateError(bool resume);
		void stateComplete();
		void threadedFunction();

	public:
		const char *stateToText(SyncThreadState state);

		SyncThread(ofxSync *sync, int32_t threadId): _threadId(threadId), _sync(sync),
			_state(SyncThreadStateClaim), _record(0), _received(0), _dataFile(0), _session(0),
			_stream(0) {
			// same name for all threads to plug ofSetLogLevel() leak
			// in startThread()
			getPocoThread().setName(BOB);
			MD5_Init(&_md5Context);
		}
	};

	friend class SyncThread;
	friend bool removeFiles(const SyncRecord *record, bool file, bool info, bool data, bool hash);

	static const int32_t DEFAULT_MAX_THREADS = 3;
	static const int32_t DEFAULT_CONNECT_TIMEOUT = 20;
	static const int32_t DEFAULT_TRANSFER_TIMEOUT = 60;
	static const bool DEFAULT_REMOVE = true;
	static const bool DEFAULT_RETRY = true;

	static int32_t _refCount;
	bool _init;
	ofMutex _mutex;
	int32_t _threadId;
	time_t _startTime;
	std::string _cacheDir;
	int32_t _maxThreads;
	int32_t _connectTimeout;
	int32_t _transferTimeout;
	bool _remove;
	bool _retry;
	SyncCallback _callback;
	void *_opaque;

	typedef std::map<std::string, SyncRecord> RecordMap;
	typedef std::pair<std::string, SyncRecord> RecordMapElement;
	typedef std::pair<RecordMap::iterator, bool> RecordMapResult;
	typedef std::list<SyncRecord *> RecordList;
	typedef std::list<SyncThread *> ThreadList;

	// the following are protected by _mutex
	RecordMap _recordMap;
	RecordList _recordQueue;
	ThreadList _livingThreads;
	ThreadList _zombieThreads;

	SyncRecord *claimSyncRecord(int32_t threadId);
	bool releaseSyncRecord(const SyncRecord *record, bool remove);
	void startThreadsLocked();
	void stopThreadsLocked();
	void threadDoneLocked(int32_t threadId);
	void zombieSlayerLocked(bool shutdown);
	void zombieSlayer(bool shutdown);

public:
	ofxSync(): _init(false), _threadId(0), _startTime(time(0)), _maxThreads(DEFAULT_MAX_THREADS),
		_connectTimeout(DEFAULT_CONNECT_TIMEOUT), _transferTimeout(DEFAULT_TRANSFER_TIMEOUT),
		_remove(DEFAULT_REMOVE), _retry(DEFAULT_RETRY), _callback(0), _opaque(0) {}
	bool initialize(const std::string &cacheDir);
	bool reload();
	void setMaxThreads(int32_t maxThreads);
	void setTimeouts(int32_t connectTimeout, int32_t transferTimeout);
	void setRemove(bool remove);
	void setRetry(bool retry);
	void setCallback(SyncCallback callback, void *opaque);
	const char *statusToText(SyncStatus status);
	bool synchronize(FileList files, bool lenient = false);
	bool shutDown();
};
