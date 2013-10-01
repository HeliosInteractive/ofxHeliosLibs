#pragma once
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <list>
#include <climits>
#include <ofMain.h>
#include <Poco/DigestEngine.h>
#include <Poco/MD5Engine.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/DNS.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include "ofxAtomicLog.h"
#include "ofxScopeMutex.h"

class ofxDownloader {
public:
	enum DownloadStatus {
		DownloadStatusAdded,
		DownloadStatusStarted,
		DownloadStatusProgress,
		DownloadStatusError,
		DownloadStatusComplete
	};

	typedef bool (*DownloadCallback)(void *opaque, DownloadStatus status, int32_t id,
		int64_t length);

private:
	friend class WorkerThread;

	class DownloadInfo {
	public:
		int32_t _threadId;
		int32_t _downloadId;
		int32_t _triesLeft;
		time_t _failureTime;
		std::string _url;
		std::string _infoPath;
		std::string _dataPath;
		std::string _filePath;
		int64_t _length;
		std::string _md5Digest;

		bool writeToFile(const std::string &path);
		bool readFromFile(const std::string &path);
	};

	class WorkerThread: public ofThread {
	public:
		enum WorkerThreadState {
			WorkerThreadStateClaim,
			WorkerThreadStateConnect,
			WorkerThreadStateDownload,
			WorkerThreadStatePersist,
			WorkerThreadStateError,
			WorkerThreadStateComplete
		};

		ofxDownloader *_downloader;
		int32_t _threadId;
		WorkerThreadState _state;
		DownloadInfo *_info;
		Poco::Net::HTTPClientSession *_session;
		std::istream *_stream;
		Poco::MD5Engine _md5Engine;
		int64_t _received;

		const char *stateToText(WorkerThreadState state);
		void threadedFunction();

		WorkerThread(ofxDownloader *downloader, int32_t threadId): _downloader(downloader),
			_threadId(threadId), _state(WorkerThreadStateClaim), _info(0), _session(0), _stream(0),
			_received(0) {
			// same name for all threads to plug ofSetLogLevel() leak
			// in startThread()
			getPocoThread().setName("bob");
		}
	};

	static const int32_t ConnectTimeout = 20;
	static const int32_t DownloadTimeout = 60;

	ofMutex _mutex;
	int32_t _threadId;
	int32_t _downloadId;
	time_t _startTime;
	bool _initialized;
	std::string _downloadDir;
	int32_t _maxThreads;
	int32_t _maxTries;
	int32_t _retryDelay;
	bool _resume;
	bool _overwrite;
	int32_t _connectTimeout;
	int32_t _downloadTimeout;
	DownloadCallback _callback;
	void *_opaque;

	// the following are protected by _mutex
	std::list<DownloadInfo> _pendingDownloads;
	std::list<WorkerThread *> _runningThreads;
	std::list<WorkerThread *> _buriedThreads;

	DownloadInfo *claimPendingDownload(int32_t threadId);
	bool releasePendingDownload(const DownloadInfo *info, bool complete);
	bool postponePendingDownload(const DownloadInfo *info);
	bool completePendingDownload(const DownloadInfo *info);
	void queuePendingDownload(const DownloadInfo &info);
	void startWorkerThreadLocked();
	void startWorkerThread();
	bool removeWorkerThreadLocked(int32_t threadId);
	void buryWorkerThreadLocked(WorkerThread *thread);
	void workerThreadAscension();
	void stopWorkerThreads();
	bool validateFile(const std::string &filePath);

public:
	ofxDownloader(): _threadId(0), _downloadId(0), _startTime(time(0)), _initialized(false) {}
	bool initialize(std::vector<int32_t> &ids, const std::string &downloadDir, int32_t maxThreads = 3,
		int32_t maxTries = 3, int32_t retryDelay = 10, bool resume = true, bool overwrite = false,
		int32_t connectTimeout = ConnectTimeout, int32_t downloadTimeout = DownloadTimeout,
		DownloadCallback callback = 0, void *opaque = 0);
	bool addDownload(int32_t &id, const std::string &url, const std::string &fileName = "",
		int64_t length = -1, const std::string &md5Digest = "");
	const char *statusToText(DownloadStatus status);
	bool waitForDownload(int32_t id);
	bool stopDownload(int32_t id);
	bool cancelDownload(int32_t id);
	bool clearDownloadDir();
	bool shutDown();
};
