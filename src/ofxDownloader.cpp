#include "ofxDownloader.h"

static const std::string makeInfoFileName(const std::string &suffix) {
	static const std::string PREFIX("!!!-");
	static const std::string EXTENSION(".dif");
	return PREFIX + suffix + EXTENSION;
}

static const std::string makeDataFileName(const std::string &suffix) {
	static const std::string PREFIX("!!!-");
	static const std::string EXTENSION(".ddf");
	return PREFIX + suffix + EXTENSION;
}

static const std::string urlToFileName(const std::string &url) {
	ofxLogVer("Extracting file name from " << url);
	std::string scheme, path;
	try {
		Poco::URI uri(url);
		scheme = uri.getScheme();
		path = uri.getPath();
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while parsing URL " << url << ": " << ex.name() << " (" << ex.message() <<
			")");
		return "";
	}
	if (scheme != "http") {
		ofxLogErr("URL " << url << " with scheme " << scheme << " is not a HTTP URL");
		return "";
	}
	size_t off = path.rfind('/');
	if (off == std::string::npos)
		off = 0;
	if (off + 1 >= path.size()) {
		ofxLogErr("Missing file name in URL path " << path);
		return "";
	}
	return path.substr(off + 1, path.size() - off - 1);
}

static const std::string md5ToString(const Poco::DigestEngine::Digest &digest) {
	static char hexDigits[17] = "0123456789abcdef";
	char digestString[32];
	int32_t i = 0;
	for (Poco::DigestEngine::Digest::const_iterator it = digest.begin(); it != digest.end(); it++) {
		digestString[2 * i] = hexDigits[(digest[i] >> 4) & 15];
		digestString[2 * i + 1] = hexDigits[digest[i] & 15];
		i++;
	}
	assert(i == sizeof digestString / 2);
	return std::string(digestString, sizeof digestString);
}

const char *ofxDownloader::WorkerThread::stateToText(WorkerThreadState state) {
	switch (state) {
	case WorkerThreadStateClaim:
		return "Claim";
	case WorkerThreadStateConnect:
		return "Connect";
	case WorkerThreadStateDownload:
		return "Download";
	case WorkerThreadStatePersist:
		return "Persist";
	case WorkerThreadStateError:
		return "Error";
	case WorkerThreadStateComplete:
		return "Complete";
	default:
		return "(Invalid)";
	}
}

static const int32_t BufferSize = 100000;

void ofxDownloader::WorkerThread::threadedFunction() {	
	ofxLogVer("Entering thread #" << _threadId);
	_downloader->workerThreadAscension();
	char buffer[BufferSize];
	ofFile dataFile;
	while (isThreadRunning()) {
		ofxLogVer("Thread #" << _threadId << " is in state " << stateToText(_state));
		switch (_state) {
		case WorkerThreadStateClaim: {
			if ((_info = _downloader->claimPendingDownload(_threadId)) == 0) {
				_state = WorkerThreadStateError;
				continue;
			}
			if (_info->_failureTime > 0) {
				ofxLogVer("Download previously failed");
				time_t until = _info->_failureTime + _downloader->_retryDelay;
				time_t now;
				while (isThreadRunning() && (now = time(0)) < until) {
					ofxLogVer("Delaying download #" << _info->_downloadId << ", " << (until - now) <<
						" second(s) left");
					sleep(1000);
				}
			}
			if (!_downloader->_resume) {
				ofxLogVer("Resumed downloads are disabled");
				ofFile file(_info->_dataPath, ofFile::Reference, true);
				if (file.exists()) {
					ofxLogVer("Removing existing data file " << _info->_dataPath);
					if (!file.remove()) {
						ofxLogErr("Error while removing existing data file " << _info->_dataPath <<
							" for download #" << _info->_downloadId);
						_state = WorkerThreadStateError;
						continue;
					}
				}
			}
			ofxLogVer("Opening data file " << _info->_dataPath);
			if (!dataFile.open(_info->_dataPath, ofFile::Append, true)) {
				ofxLogErr("Error while opening data file " << _info->_dataPath << " for download #" <<
					_info->_downloadId);
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogNot("Thread #" << _threadId << " claimed download #" << _info->_downloadId <<
				" from " << _info->_url);
			_state = WorkerThreadStateConnect;
			break;
		}
		case WorkerThreadStateConnect: {
			ofxLogVer("Connecting download #" << _info->_downloadId << " to " << _info->_url);
			std::string host, path;
			int32_t port;
			try {
				Poco::URI uri(_info->_url);
				host = uri.getHost();
				path = uri.getPathAndQuery();
				port = uri.getPort();
			} catch (Poco::Exception &ex) {
				ofxLogErr("Error while parsing URL " << _info->_url << " for download #" <<
					_info->_downloadId << ": " << ex.name() << " (" << ex.message() << ")");
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogVer("Resolving server DNS name " << host);
			Poco::Net::IPAddress ipAddress;
			try {
				ipAddress = Poco::Net::DNS::hostByName(host).addresses()[0];
			} catch (Poco::Exception &ex) {
				ofxLogErr("Error while resolving server DNS name " << host << " for download #" <<
					_info->_downloadId << ": " << ex.name() << " (" << ex.message() << ")");
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogVer("Initializing HTTP session with IP address " << ipAddress.toString() <<
				" and port " << port);
			_session = new Poco::Net::HTTPClientSession();
			_session->setHost(host);
			_session->setPort(port);
			_session->setKeepAlive(false);
			_session->setTimeout(_downloader->_connectTimeout * 1000 * 1000);
			ofxLogVer("Creating HTTP request for " << path);
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path,
				Poco::Net::HTTPMessage::HTTP_1_1);
			ofxLogVer("Sending HTTP request");
			try {
				_session->sendRequest(request);
			} catch (Poco::Exception &ex) {
				ofxLogErr("Error while connecting to IP address " << ipAddress.toString() << " (" <<
					host << ") for download #" << _info->_downloadId << ": " << ex.name() << " (" <<
					ex.message() << ")");
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogVer("Receiving HTTP response");
			Poco::Net::HTTPResponse response;
			try {
				_stream = &_session->receiveResponse(response);
			} catch (Poco::Exception &ex) {
				ofxLogErr("Error while receiving HTTP response from " << host << " for download #" <<
					_info->_downloadId << ": " << ex.name() << " (" << ex.message() << ")");
				_state = WorkerThreadStateError;
				continue;
			}
			switch (response.getStatus()) {
			case Poco::Net::HTTPResponse::HTTP_OK: {
				ofxLogVer("Received a 200 OK response");
				break;
			}
			case Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY:
			case Poco::Net::HTTPResponse::HTTP_FOUND:
			case Poco::Net::HTTPResponse::HTTP_SEE_OTHER:
			case Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT: {
				if (!response.has("Location")) {
					ofxLogErr("HTTP " << response.getStatus() << " redirection without location header "
						" from " << host << " for download #" << _info->_downloadId);
					_state = WorkerThreadStateError;
					continue;
				}
				ofxLogVer("HTTP " << response.getStatus() << " redirection to " <<
					response["Location"] <<	" from " << host << " for download #" << _info->_downloadId);
				_info->_url = response["Location"];
				// POCO's HTTP session doesn't purge the 3xx response's body,
				// so don't re-use the existing session
				delete _session;
				_session = 0;
				// stay in WorkerThreadStateConnect and reconnect with new URL
				continue;
			}
			default: {
				ofxLogErr("HTTP response with invalid HTTP status code " << response.getStatus() <<
					" from " << host << " for download #" << _info->_downloadId);
				_state = WorkerThreadStateError;
				continue;
			}}
			int64_t contentLength = response.getContentLength64();
			if (contentLength != Poco::Net::HTTPResponse::UNKNOWN_CONTENT_LENGTH) {
				ofxLogVer("HTTP response has Content-Length header");
				if (_info->_length < 0) {
					ofxLogVer("Accepting content length " << contentLength << " from " << host <<
						" for download #" << _info->_downloadId);
					_info->_length = contentLength;
				} else if (_info->_length != contentLength) {
					ofxLogErr("Length mismatch between known length " << _info->_length << " and "
						"content length " << contentLength << " from " << host << " for download #" <<
						_info->_downloadId);
					_state = WorkerThreadStateError;
					continue;
				}
			} else {
				ofxLogVer("HTTP response without Content-Length header");
				if (_info->_length < 0 && _info->_md5Digest.empty()) {
					ofxLogErr("HTTP response from " << host << " without Content-Length header for "
						"download  #" << _info->_downloadId << " without known length or MD5 digest");
					_state = WorkerThreadStateError;
					continue;
				}
			}
			_session->socket().setReceiveTimeout(_downloader->_downloadTimeout * 1000 * 1000);
			_state = WorkerThreadStateDownload;
			break;
		}
		case WorkerThreadStateDownload: {
			ofxLogVer("Receiving download #" << _info->_downloadId);
			// POCO doesn't make a difference between regular EOF and connection
			// or HTTP protocol errors; rely on file size and MD5 digest to detect
			// transmission problems
			if (_stream->good()) {
				_stream->read(buffer, BufferSize);
				streamsize count = _stream->gcount();
				_received += count;
				ofxLogVer("Received " << count << " (total: " << _received << ") byte(s)");
				_md5Engine.update(buffer, count);
				if (!dataFile.writeFromBuffer(ofBuffer(buffer, count))) {
					ofxLogErr("Error while writing " << count << " byte(s) to data file " <<
						_info->_dataPath << " for download #" << _info->_downloadId);
					_state = WorkerThreadStateError;
					continue;
				}
			} else {
				ofxLogVer("End of stream or error for download #" << _info->_downloadId);
				if (_info->_length >= 0 && _received != _info->_length) {
					ofxLogErr("Unexpected amount of received data for download #" << _info->_downloadId <<
						" (" << _received << " vs. " << _info->_length << ")");
					_state = WorkerThreadStateError;
					continue;
				} else if (_info->_md5Digest.size() > 0) {
					const std::string &digest = md5ToString(_md5Engine.digest());
					if (digest != _info->_md5Digest) {
						ofxLogErr("MD5 digest mismatch for download #" << _info->_downloadId << " (" <<
							digest << " vs. " << _info->_md5Digest << ")");
						_state = WorkerThreadStateError;
						continue;
					}
				}
				ofxLogVer("Download #" << _info->_downloadId << " passed length and MD5 digest checks");
				_state = WorkerThreadStatePersist;
			}
			break;
		}
		case WorkerThreadStatePersist: {
			ofxLogVer("Persisting download #" << _info->_downloadId << " to " << _info->_filePath);
			dataFile.close();
			if (_downloader->_overwrite) {
				ofxLogVer("Overwriting existing file");
				ofFile::removeFile(_info->_filePath, false);
			}
			if (!ofFile::moveFromTo(_info->_dataPath, _info->_filePath, false, true)) {
				ofxLogErr("Error while renaming data file " << _info->_dataPath << " to " <<
					_info->_filePath << " for download #" << _info->_downloadId);
				_state = WorkerThreadStateError;
				continue;
			}
			if (!ofFile::removeFile(_info->_infoPath, false)) {
				ofxLogErr("Error while removing info file " << _info->_infoPath << " for download #" <<
					_info->_downloadId);
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogVer("Download #" << _info->_downloadId << " persisted");
			_state = WorkerThreadStateComplete;
			break;
		}
		case WorkerThreadStateError: {
			if (_info == 0)
#warning FIX LOGGING MACRO
				ofxLogErr("Download failed on thread #" << _threadId) // no semicolon
			else {
				ofxLogErr("Download #" << _info->_downloadId << " from " << _info->_url <<
					" failed on thread #" << _threadId);
				_info->_failureTime = time(0);
				_info->_triesLeft--;
				ofxLogVer("Download #" << _info->_downloadId << " has " << _info->_triesLeft <<
					" tr[y|ies] left");
				if (_info->_triesLeft > 0)
					_downloader->postponePendingDownload(_info);
				else
					_downloader->completePendingDownload(_info);
			}
			goto leave;
		}
		case WorkerThreadStateComplete: {
			ofxLogNot("Download #" << _info->_downloadId << " from " << _info->_url <<
				" successfully completed on thread #" << _threadId);
			_downloader->completePendingDownload(_info);
			goto leave;
		}}
	}
leave:
	if (_session != 0) {
		ofxLogVer("Deleting HTTP session");
		delete _session;
		_session = 0;
	}
	if (dataFile.is_open()) {
		ofxLogVer("Closing data file");
		dataFile.close();
	}
	{
		ofxScopeMutex m(_downloader->_mutex);
		if (isThreadRunning()) {
			_downloader->removeWorkerThreadLocked(_threadId);
			_downloader->buryWorkerThreadLocked(this);
			int32_t numDo = _downloader->_pendingDownloads.size();
			int32_t numTh = _downloader->_runningThreads.size();
			ofxLogVer(numDo << " pending download(s), " << numTh << " running thread(s)");
			if (numDo > numTh)
				_downloader->startWorkerThreadLocked();
		} else
			ofxLogVer("No removal, burials, or new threads during shutdown");
	}
	ofxLogVer("Leaving thread #" << _threadId);
}

ofxDownloader::DownloadInfo *ofxDownloader::claimPendingDownload(int32_t threadId) {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Thread #" << threadId << " trying to claim a download");
	for (std::list<DownloadInfo>::iterator it = _pendingDownloads.begin();
		it != _pendingDownloads.end(); it++) {
		if (it->_threadId < 0) {
			ofxLogVer("Thread #" << _threadId << " claimed download #" << it->_downloadId);
			it->_threadId = threadId;
			return &*it;
		}
	}
	ofxLogErr("AMAZING! Thread #" << _threadId << " did not find any download to claim");
	return 0;
}

bool ofxDownloader::releasePendingDownload(const DownloadInfo *info, bool complete) {
	ofxScopeMutex m(_mutex);
	for (std::list<DownloadInfo>::iterator it = _pendingDownloads.begin();
		it != _pendingDownloads.end(); it++)
		if (&*it == info) {
			if (complete)
				_pendingDownloads.erase(it);
			else {
				it->_threadId = -1;
				_pendingDownloads.splice(_pendingDownloads.end(), _pendingDownloads, it);
			}
			return true;
		}
	return false;
}

bool ofxDownloader::postponePendingDownload(const DownloadInfo *info) {
	int32_t threadId = info->_threadId, downloadId = info->_downloadId;
	ofxLogVer("Thread #" << threadId << " postponing download #" << downloadId);
	if (releasePendingDownload(info, false)) {
		ofxLogVer("Thread #" << threadId << " postponed download #" << downloadId);
		return true;
	}
	ofxLogErr("AMAZING! Thread #" << threadId << " postponed non-existent download #" << downloadId);
	return false;
}

bool ofxDownloader::completePendingDownload(const DownloadInfo *info) {
	int32_t threadId = info->_threadId, downloadId = info->_downloadId;
	ofxLogVer("Thread #" << threadId << " completing download #" << downloadId);
	if (releasePendingDownload(info, true)) {
		ofxLogVer("Thread #" << threadId << " completed download #" << downloadId);
		return true;
	}
	ofxLogErr("AMAZING! Thread #" << threadId << " completed non-existent download #" << downloadId);
	return false;
}

void ofxDownloader::queuePendingDownload(const DownloadInfo &info) {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Queueing download #" << info._downloadId);
	_pendingDownloads.push_back(info);
}

void ofxDownloader::startWorkerThreadLocked() {
	ofxLogVer("Checking number of threads");
	if (static_cast<int32_t>(_runningThreads.size()) < _maxThreads) {
		WorkerThread *thread = new WorkerThread(this, ++_threadId);
		_runningThreads.push_back(thread);
		ofxLogVer("Created new thread #" << _threadId << ", now " << _runningThreads.size() <<
			" thread(s) of " << _maxThreads);
		thread->startThread();
	} else
		ofxLogVer(_runningThreads.size() << " thread(s) running already");	
}

void ofxDownloader::startWorkerThread() {
	ofxScopeMutex m(_mutex);
	startWorkerThreadLocked();
}

bool ofxDownloader::removeWorkerThreadLocked(int32_t threadId) {
	ofxLogVer("Removing thread #" << threadId);
	for (std::list<WorkerThread *>::iterator it = _runningThreads.begin();
		it != _runningThreads.end(); it++) {
		if ((*it)->_threadId == threadId) {
			_runningThreads.erase(it);
			ofxLogVer("Thread #" << threadId << " removed");
			return true;
		}
	}
	ofxLogErr("AMAZING! Thread #" << threadId << " not found");
	return false;
}

void ofxDownloader::buryWorkerThreadLocked(WorkerThread *thread) {
	ofxLogVer("Burying thread #" << thread->_threadId);
	_buriedThreads.push_back(thread);
}

void ofxDownloader::stopWorkerThreads() {
	ofxLogVer("Stopping all worker threads");
	while (true) {
		WorkerThread *thread;
		{
				ofxScopeMutex m(_mutex);
				ofxLogVer("Checking for worker threads");
				if (_runningThreads.size() == 0)
					break;
				thread = _runningThreads.front();
				_runningThreads.pop_front();
				ofxLogVer("Stopping thread #" << thread->_threadId);
				thread->stopThread();
		}
		// waitForThread() doesn't join, if the thread is not running anymore,
		// which leads to a pthread resource leak
		thread->getPocoThread().tryJoin(10 * 1000);
		thread->waitForThread();
		ofxLogVer("Thread #" << thread->_threadId << " stopped");
		delete thread;
	}
}

void ofxDownloader::workerThreadAscension() {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Worker thread ascension is nigh");
	for (std::list<WorkerThread *>::iterator it = _buriedThreads.begin();
		it != _buriedThreads.end(); it++) {
		ofxLogVer("Thread #" << (*it)->_threadId << " ascending");
		// see stopWorkerThreads()
		(*it)->getPocoThread().tryJoin(10 * 1000);
		(*it)->waitForThread();
		ofxLogVer("Thread #" << (*it)->_threadId << " ascended");
		delete *it;
	}
	_buriedThreads.clear();
}

bool ofxDownloader::validateFile(const std::string &filePath) {
	ofxLogVer("Validating file " << filePath);
	for (std::list<DownloadInfo>::iterator it = _pendingDownloads.begin();
		it != _pendingDownloads.end(); it++) {
		if (it->_filePath == filePath) {
			ofxLogErr("File " << filePath << " already covered by download #" << it->_downloadId);
			return false;
		}
	}
	if (_overwrite)
		return true;
	ofFile fileFile(filePath, ofFile::Reference, true);
	if (fileFile.exists()) {
		ofxLogErr("File " << filePath << " already exists");
		return false;
	}
	return true;
}

bool ofxDownloader::initialize(std::vector<int32_t> &ids, const std::string &downloadDir,
	int32_t maxThreads, int32_t maxTries, int32_t retryDelay, bool resume, bool overwrite,
	int32_t connectTimeout, int32_t downloadTimeout, DownloadCallback callback, void *opaque) {
	ofxLogVer("Initializing downloader");
	if (_initialized) {
		ofxLogErr("Downloader already initialized");
		return false;
	}
	ofDirectory dir(downloadDir);
	if (!dir.exists()) {
		ofxLogErr("Download directory " << downloadDir << " does not exist");
		return false;
	}
	if (!dir.isDirectory()) {
		ofxLogErr("Download directory " << downloadDir << " is not a directory");
		return false;
	}
	_downloadDir = downloadDir;
	_maxThreads = maxThreads;
	_maxTries = maxTries;
	_retryDelay = retryDelay;
	_resume = resume;
	_overwrite = overwrite;
	_connectTimeout = connectTimeout;
	_downloadTimeout = downloadTimeout;
	_callback = callback;
	_opaque = opaque;
	_pendingDownloads.clear();
	_runningThreads.clear();
	_buriedThreads.clear();
	dir.allowExt("dif");
	int32_t count = dir.listDir();
	ofxLogVer("Found " << count << " info file(s) in " << downloadDir);
	std::vector<int32_t> tmpIds;
	for (int32_t i = 0; i < count; i++) {
		DownloadInfo info;
		info._threadId = -1;
		info._downloadId = ++_downloadId;
		tmpIds.push_back(info._downloadId);
		info._triesLeft = _maxTries;
		info._failureTime = 0;
		const std::string &path = dir.getPath(i);
		if (!info.readFromFile(path)) {
			ofxLogErr("Error while loading info file " << path);
			return false;
		}
		if (!validateFile(info._filePath)) {
			ofxLogErr("Error while validating file " << info._filePath);
			return false;
		}
		queuePendingDownload(info);
		startWorkerThread();
	}
	ids = tmpIds;
	_initialized = true;
	return true;
}

bool ofxDownloader::DownloadInfo::writeToFile(const std::string &path) {
	ofxLogVer("Storing download info in " << path);
	ofFile file;
	if (!file.open(path, ofFile::WriteOnly, true)) {
		ofxLogErr("Error while opening info file " << path << " for writing");
		return false;
	}
	file << _url << std::endl;
	file << _infoPath << std::endl;
	file << _dataPath << std::endl;
	file << _filePath << std::endl;
	file << _length << std::endl;
	file << _md5Digest << std::endl;
	if (!file.good()) {
		ofxLogErr("Error while writing to info file " << path);
		return false;
	}
	return true;
}

bool ofxDownloader::DownloadInfo::readFromFile(const std::string &path) {
	ofxLogVer("Loading download info from info file " << path);
	ofFile file;
	if (!file.open(path, ofFile::ReadOnly, true)) {
		ofxLogErr("Error while opening info file " << path << " for reading");
		return false;
	}
	std::getline(file, _url);
	std::getline(file, _infoPath);
	std::getline(file, _dataPath);
	std::getline(file, _filePath);
	std::string length;
	std::getline(file, length);
	std::istringstream iss(length);
	if (!(iss >> _length)) {
		ofxLogErr("Invalid length in info file " << path);
		return false;
	}
	std::getline(file, _md5Digest);
	if (!file.good()) {
		ofxLogErr("Error while reading from info file " << path);
		return false;
	}
	return true;
}

bool ofxDownloader::addDownload(int32_t &id, const std::string &url, const std::string &fileName,
	int64_t length, const std::string &md5Digest) {
	ofxLogVer("Adding download from " << url);
	if (!_initialized) {
		ofxLogErr("Downloader not initialized");
		return false;
	}
	const std::string &realFileName = fileName.size() == 0 ? urlToFileName(url) : fileName;
	if (realFileName.size() == 0)
		return false;
	const std::string &filePath = ofFilePath::join(_downloadDir, realFileName);
	ofxLogVer("Downloading to " << filePath);
	if (!validateFile(filePath)) {
		ofxLogErr("Error while validating file " << filePath);
		return false;
	}
	Poco::MD5Engine md5Engine;
	md5Engine.update(&_startTime, sizeof _startTime);
	md5Engine.update(&_downloadId, sizeof _downloadId);
	md5Engine.update(url);
	md5Engine.update(fileName);
	const std::string &suffix = md5ToString(md5Engine.digest());
	const std::string &infoPath = ofFilePath::join(_downloadDir, makeInfoFileName(suffix));
	const std::string &dataPath = ofFilePath::join(_downloadDir, makeDataFileName(suffix));
	ofxLogVer("Using info file " << infoPath << ", data file " << dataPath);
	ofFile infoFile(infoPath, ofFile::Reference, true);
	if (infoFile.exists()) {
		ofxLogErr("AMAZING! Info file " << infoPath << " already exists");
		return false;
	}
	ofFile dataFile(dataPath, ofFile::Reference, true);
	if (dataFile.exists()) {
		ofxLogErr("AMAZING! Data file " << dataPath << " already exists");
		return false;
	}
	DownloadInfo info;
	info._threadId = -1;
	info._downloadId = ++_downloadId;
	info._triesLeft = _maxTries;
	info._failureTime = 0;
	info._url = url;
	info._infoPath = infoPath;
	info._dataPath = dataPath;
	info._filePath = filePath;
	info._length = length;
	info._md5Digest = md5Digest;
	if (!info.writeToFile(infoPath)) {
		ofxLogErr("Error while storing info file " << infoPath);
		return false;
	}
	queuePendingDownload(info);
	startWorkerThread();
	id = _downloadId;
	return true;
}

const char *ofxDownloader::statusToText(DownloadStatus status) {
	switch (status) {
	case DownloadStatusAdded:
		return "Added";
	case DownloadStatusStarted:
		return "Started";
	case DownloadStatusProgress:
		return "Progress";
	case DownloadStatusError:
		return "Error";
	case DownloadStatusComplete:
		return "Complete";
	default:
		return "(Invalid)";
	}
}

bool ofxDownloader::waitForDownload(int32_t id) {
	return false;
}

bool ofxDownloader::stopDownload(int32_t id) {
	return false;
}

bool ofxDownloader::cancelDownload(int32_t id) {
	return false;
}

bool ofxDownloader::clearDownloadDir() {
	return false;
}

bool ofxDownloader::shutDown() {
	ofxLogVer("Shutting down downloader");
	if (!_initialized) {
		ofxLogErr("Downloader not initialized");
		return false;
	}
	stopWorkerThreads();
	workerThreadAscension();
	_pendingDownloads.clear();
	_initialized = false;
	return false;
}
