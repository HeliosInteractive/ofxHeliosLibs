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

static const std::string makeHashFileName(const std::string &suffix) {
	static const std::string PREFIX("!!!-");
	static const std::string EXTENSION(".dhf");
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

static const std::string md5ToString(const uint8_t md5Result[16]) {
	static char hexDigits[17] = "0123456789abcdef";
	char digestString[32];
	for (int32_t i = 0; i < 16; i++) {
		digestString[2 * i] = hexDigits[md5Result[i] >> 4];
		digestString[2 * i + 1] = hexDigits[md5Result[i] & 15];
	}
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

static const int32_t BufferSize = 10000;

bool ofxDownloader::WorkerThread::readHashFile(MD5_CTX &md5Context, const std::string &path) {
	ofxLogVer("Loading MD5 context from " << path);
	ofFile file;
	if (!file.open(path, ofFile::ReadOnly, true)) {
		ofxLogErr("Error while opening hash file " << path << " for reading");
		return false;
	}
	const ofBuffer &buffer = file.readToBuffer();
	if (buffer.size() != sizeof md5Context) {
		ofxLogErr("Unexpected amount of data read from hash file " << path << "(" << buffer.size() <<
			" vs. " << sizeof md5Context);
		return false;
	}
	memcpy(&md5Context, buffer.getBinaryBuffer(), sizeof md5Context);
	return true;
}

bool ofxDownloader::WorkerThread::writeHashFile(const MD5_CTX &md5Context, const std::string &path) {
	ofxLogVer("Storing MD5 context in " << path);
	ofFile file;
	if (!file.open(path, ofFile::WriteOnly, true)) {
		ofxLogErr("Error while opening hash file " << path << " for writing");
		return false;
	}
	if (!file.writeFromBuffer(ofBuffer(reinterpret_cast<const char *>(&md5Context),
		sizeof md5Context)) || !file.good()) {
		ofxLogErr("Error while writing to hash file " << path);
		return false;
	}
	return true;
}

void ofxDownloader::WorkerThread::threadedFunction() {	
	ofxLogVer("Entering thread #" << _threadId);
	_downloader->workerThreadAscension();
	char buffer[BufferSize];
	ofFile dataFile;
	bool done = false;
	while (!done && isThreadRunning()) {
		ofxLogVer("Thread #" << _threadId << " is in state " << stateToText(_state));
		switch (_state) {
		case WorkerThreadStateClaim: {
			if ((_info = _downloader->claimPendingDownload(_threadId)) == 0) {
				// resume
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
			ofFile exDataFile(_info->_dataPath, ofFile::Reference, true);
			ofFile exHashFile(_info->_hashPath, ofFile::Reference, true);
			if (exDataFile.exists()) {
				ofxLogVer("Data file " << _info->_dataPath << " exists already");
				if (!_info->_resume) {
					ofxLogVer("Removing existing data file " << _info->_dataPath);
					if (!exDataFile.remove()) {
						ofxLogErr("Error while removing existing data file " << _info->_dataPath <<
							" for download #" << _info->_downloadId);
						_info->_resume = false;
						_state = WorkerThreadStateError;
						continue;
					}
				} else {
					_received = exDataFile.getSize();
					ofxLogVer("Keeping existing data file " << _info->_dataPath << " with " <<
						_received << " byte(s)");
				}
			}
			if (_received > 0) {
				ofxLogVer("Looking for hash file " << _info->_hashPath << " for data file " <<
					_info->_dataPath);
				if (!exHashFile.exists()) {
					ofxLogErr("Missing hash file " << _info->_hashPath << " for data file " <<
						_info->_dataPath << " with length " << _received << " for download #" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				if (!readHashFile(_md5Context, _info->_hashPath)) {
					ofxLogErr("Error while reading hash file " << _info->_hashPath << " for download #" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
			} else {
				MD5_Init(&_md5Context);
				if (exHashFile.exists()) {
					ofxLogVer("Removing existing hash file " << _info->_hashPath);
					if (!exHashFile.remove()) {
						ofxLogErr("Error while removing existing hash file " << _info->_hashPath <<
							"for download #" << _info->_downloadId);
						_info->_resume = false;
						_state = WorkerThreadStateError;
						continue;
					}
				}
			}
			ofxLogVer("Opening data file " << _info->_dataPath);
			if (!dataFile.open(_info->_dataPath, ofFile::Append, true)) {
				ofxLogErr("Error while opening data file " << _info->_dataPath << " for download #" <<
					_info->_downloadId);
				_info->_resume = false;
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogNot("Thread #" << _threadId << " claimed download #" << _info->_downloadId <<
				" from " << _info->_url);
			_state = WorkerThreadStateConnect;
			if (_downloader->_callback != 0)
				_downloader->_callback(_downloader->_opaque, DownloadStatusStarted, _info->_downloadId,
					_received, _info->_length, _downloader->_maxTries - _info->_triesLeft + 1);
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
				// resume
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
				// resume
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
			if (_info->_resume && _received > 0) {
				ofxLogVer("Adding HTTP range header");
				ostringstream oss;
				oss << "bytes=" << _received << "-";
				request.add("Range", oss.str());
			}
			ofxLogVer("Sending HTTP request");
			try {
				_session->sendRequest(request);
			} catch (Poco::Exception &ex) {
				ofxLogErr("Error while connecting to IP address " << ipAddress.toString() << " (" <<
					host << ") for download #" << _info->_downloadId << ": " << ex.name() << " (" <<
					ex.message() << ")");
				// resume
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
				// resume
				_state = WorkerThreadStateError;
				continue;
			}
			switch (response.getStatus()) {
			case Poco::Net::HTTPResponse::HTTP_OK: {
				ofxLogVer("Received a 200 OK response");
				if (_info->_resume && _received > 0) {
					ofxLogErr(host << " failed to resume download #" << _info->_downloadId <<
						" from " << _info->_url);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				break;
			}
			case Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT: {
				ofxLogVer("Received a 206 Partial Content response");
				if (!_info->_resume || _received == 0) {
					ofxLogErr("Unexpected partial content from " << host << " for download #" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				if (!response.has("Content-Range")) {
					ofxLogErr("Missing Content-Range header from " << host << " for download #" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				ostringstream oss;
				oss << "bytes " << _received << "-";
				const std::string &expected = oss.str();
				const std::string &actual = response.get("Content-Range", "");
				ofxLogVer("Content-Range header is " << actual);
				if (actual.size() <= expected.size() || actual.substr(0, expected.size()) != expected) {
					ofxLogErr("Unexpected Content-Range header from " << host << " for download #" <<
						_info->_downloadId << " (" << actual << " vs. " << expected << "*)");
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				break;
			}
			case Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY:
			case Poco::Net::HTTPResponse::HTTP_FOUND:
			case Poco::Net::HTTPResponse::HTTP_SEE_OTHER:
			case Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT: {
				if (!response.has("Location")) {
					ofxLogErr("HTTP " << response.getStatus() << " redirection without location header"
						" from " << host << " for download #" << _info->_downloadId);
					_info->_resume = false;
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
				// resume
				_state = WorkerThreadStateError;
				continue;
			}}
			int64_t contentLength = response.getContentLength64();
			if (contentLength != Poco::Net::HTTPResponse::UNKNOWN_CONTENT_LENGTH) {
				ofxLogVer("HTTP response has Content-Length header");
				if (_info->_resume && _received > 0) {
					ofxLogVer("Adjusting content length of partial content from " << contentLength <<
						" to " << (contentLength + _received));
					contentLength += _received;
					ostringstream oss;
					oss << "/" << contentLength;
					const std::string &expected = oss.str();
					const std::string &actual = response.get("Content-Range", "");
					if (actual.size() <= expected.size() ||
						actual.substr(actual.size() - expected.size()) != oss.str()) {
						ofxLogErr("Unexpected Content-Range header from " << host << " for download #" <<
							_info->_downloadId << " (" << actual << " vs. *" << expected << ")");
						_info->_resume = false;
						_state = WorkerThreadStateError;
						continue;
					}
				}
				if (_info->_length < 0) {
					ofxLogVer("Accepting content length " << contentLength << " from " << host <<
						" for download #" << _info->_downloadId);
					_info->_length = contentLength;
				} else if (_info->_length != contentLength) {
					ofxLogErr("Length mismatch between known length " << _info->_length << " and "
						"content length " << contentLength << " from " << host << " for download #" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
			} else {
				ofxLogVer("HTTP response without Content-Length header");
				if (_info->_length < 0 && _info->_md5Digest.empty()) {
					ofxLogErr("HTTP response from " << host << " without Content-Length header for "
						"download #" << _info->_downloadId << " without known length or MD5 digest");
					_info->_resume = false;
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
				MD5_Update(&_md5Context, buffer, count);
				if (!dataFile.writeFromBuffer(ofBuffer(buffer, count))) {
					ofxLogErr("Error while writing " << count << " byte(s) to data file " <<
						_info->_dataPath << " for download #" << _info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				if (!writeHashFile(_md5Context, _info->_hashPath)) {
					ofxLogErr("Error while updating hash file " << _info->_hashPath << " for download#" <<
						_info->_downloadId);
					_info->_resume = false;
					_state = WorkerThreadStateError;
					continue;
				}
				if (false && _received == 10000) {
					// resume for testing
					_state = WorkerThreadStateError;
					continue;
				}
				if (_downloader->_callback != 0)
					_downloader->_callback(_downloader->_opaque, DownloadStatusProgress,
						_info->_downloadId, _received, _info->_length,
						_downloader->_maxTries - _info->_triesLeft + 1);
			} else {
				ofxLogVer("End of stream or error for download #" << _info->_downloadId);
				if (_info->_length >= 0 && _received != _info->_length) {
					ofxLogErr("Unexpected amount of received data for download #" << _info->_downloadId <<
						" (" << _received << " vs. " << _info->_length << ")");
					// resume
					_state = WorkerThreadStateError;
					continue;
				} else if (_info->_md5Digest.size() > 0) {
					uint8_t md5Result[16];
					MD5_Final(md5Result, &_md5Context);
					const std::string &digest = md5ToString(md5Result);
					if (digest != _info->_md5Digest) {
						ofxLogErr("MD5 digest mismatch for download #" << _info->_downloadId << " (" <<
							digest << " vs. " << _info->_md5Digest << ")");
						// don't resume, if the length is correct, but the MD5
						// digest is wrong
						if (_info->_length >= 0 && _received == _info->_length)
							_info->_resume = false;
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
				_info->_resume = false;
				_state = WorkerThreadStateError;
				continue;
			}
			if (!ofFile::removeFile(_info->_hashPath, false)) {
				ofxLogErr("Error while removing hash file " << _info->_hashPath << " for download #" <<
					_info->_downloadId);
				_info->_resume = false;
				_state = WorkerThreadStateError;
				continue;
			}
			if (!ofFile::removeFile(_info->_infoPath, false)) {
				ofxLogErr("Error while removing info file " << _info->_infoPath << " for download #" <<
					_info->_downloadId);
				_info->_resume = false;
				_state = WorkerThreadStateError;
				continue;
			}
			ofxLogVer("Download #" << _info->_downloadId << " persisted");
			_state = WorkerThreadStateComplete;
			break;
		}
		case WorkerThreadStateError: {
			if (_info == 0)
				ofxLogErr("Download failed on thread #" << _threadId);
			else {
				ofxLogErr("Download #" << _info->_downloadId << " from " << _info->_url <<
					" failed on thread #" << _threadId);
				_info->_failureTime = time(0);
				_info->_triesLeft--;
				ofxLogVer("Download #" << _info->_downloadId << " has " << _info->_triesLeft <<
					" tr[y|ies] left");
				if (_info->_triesLeft > 0) {
					if (_downloader->_callback != 0)
						_downloader->_callback(_downloader->_opaque, DownloadStatusFailure,
							_info->_downloadId, _received, _info->_length,
							_downloader->_maxTries - _info->_triesLeft);
					_downloader->postponePendingDownload(_info);
				} else {
					if (dataFile.is_open()) {
						ofxLogVer("Closing data file");
						dataFile.close();
					}
					ofxLogVer("Removing info, data, and hash files");
					if (!ofFile::removeFile(_info->_infoPath, false)) {
						ofxLogErr("Error while removing info file " << _info->_infoPath);						
					}
					ofFile rmDataFile(_info->_dataPath, ofFile::Reference, true);
					ofFile rmHashFile(_info->_hashPath, ofFile::Reference, true);
					if (rmDataFile.exists() && !rmDataFile.remove()) {
						ofxLogErr("Error while removing data file " << _info->_dataPath);						
					}
					if (rmHashFile.exists() && !rmHashFile.remove()) {
						ofxLogErr("Error while removing hash file " << _info->_hashPath);						
					}
					if (_downloader->_callback != 0)
						_downloader->_callback(_downloader->_opaque, DownloadStatusGivingUp,
							_info->_downloadId, _received, _info->_length,
							_downloader->_maxTries - _info->_triesLeft);
					_downloader->completePendingDownload(_info);
				}
			}
			done = true;
			break;
		}
		case WorkerThreadStateComplete: {
			ofxLogNot("Download #" << _info->_downloadId << " from " << _info->_url <<
				" successfully completed on thread #" << _threadId);
			if (_downloader->_callback != 0)
				_downloader->_callback(_downloader->_opaque, DownloadStatusComplete, _info->_downloadId,
					_received, _info->_length, _downloader->_maxTries - _info->_triesLeft + 1);
			_downloader->completePendingDownload(_info);
			done = true;
			break;
		}}
	}	
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
			ofxLogVer(numDo << " pending download(s), " << _downloader->_unavailable <<
				" unavailable, " << numTh << " running thread(s)");
			if (numDo > numTh + _downloader->_unavailable)
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
		if (it->_threadId < 0 && it->_available) {
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
			if (complete) {
				_pendingDownloads.erase(it);
				ofxLogVer(_pendingDownloads.size() << " pending download(s) [-]");
			} else {
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
	ofxLogVer(_pendingDownloads.size() << " pending download(s) [+]");
}

void ofxDownloader::startWorkerThreadLocked() {
	ofxLogVer("Checking number of threads");
	if (static_cast<int32_t>(_runningThreads.size()) < _maxThreads) {
		WorkerThread *thread = new WorkerThread(this, ++_threadId);
		_runningThreads.push_back(thread);
		ofxLogVer("Created new thread #" << _threadId << ", now " << _runningThreads.size() <<
			" thread(s) of " << _maxThreads);
		ofxLogVer(_runningThreads.size() << " running thread(s) [+]");
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
			ofxLogVer(_runningThreads.size() << " running thread(s) [-]");
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
	ofxScopeMutex m(_mutex);
	ofxLogVer("Stopping all worker threads");
	while (_runningThreads.size() > 0) {
		WorkerThread *thread = _runningThreads.front();
		_runningThreads.pop_front();
		ofxLogVer("Stopping thread #" << thread->_threadId);
		thread->stopThread();
		_mutex.unlock();
		// waitForThread() doesn't join, if the thread is not running anymore,
		// which leads to a pthread resource leak
		thread->getPocoThread().tryJoin(10 * 1000);
		thread->waitForThread();
		_mutex.lock();
		ofxLogVer("Thread #" << thread->_threadId << " stopped");
		delete thread;
	}
	ofxLogVer("All worker threads stopped");
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
		info._available = true;
		info._downloadId = ++_downloadId;
		tmpIds.push_back(info._downloadId);
		info._triesLeft = _maxTries;
		info._failureTime = 0;
		info._resume = _resume;
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
	file << _hashPath << std::endl;
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
	std::getline(file, _hashPath);
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
	MD5_CTX md5Context;
	MD5_Init(&md5Context);
	MD5_Update(&md5Context, &_startTime, sizeof _startTime);
	MD5_Update(&md5Context, &_downloadId, sizeof _downloadId);
	MD5_Update(&md5Context, url.data(), url.size());
	MD5_Update(&md5Context, fileName.data(), fileName.size());
	uint8_t md5Result[16];
	MD5_Final(md5Result, &md5Context);
	const std::string &suffix = md5ToString(md5Result);
	const std::string &infoPath = ofFilePath::join(_downloadDir, makeInfoFileName(suffix));
	const std::string &dataPath = ofFilePath::join(_downloadDir, makeDataFileName(suffix));
	const std::string &hashPath = ofFilePath::join(_downloadDir, makeHashFileName(suffix));
	ofxLogVer("Using info file " << infoPath << ", data file " << dataPath << ", hash file " <<
		hashPath);
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
	ofFile hashFile(hashPath, ofFile::Reference, true);
	if (hashFile.exists()) {
		ofxLogErr("AMAZING! Hash file " << hashPath << " already exists");
		return false;
	}
	DownloadInfo info;
	info._threadId = -1;
	info._available = true;
	info._downloadId = ++_downloadId;
	info._triesLeft = _maxTries;
	info._failureTime = 0;
	info._resume = _resume;
	info._url = url;
	info._infoPath = infoPath;
	info._dataPath = dataPath;
	info._hashPath = hashPath;
	info._filePath = filePath;
	info._length = length;
	info._md5Digest = md5Digest;
	std::transform(info._md5Digest.begin(), info._md5Digest.end(), info._md5Digest.begin(), ::tolower);
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
	case DownloadStatusStarted:
		return "Started";
	case DownloadStatusProgress:
		return "Progress";
	case DownloadStatusFailure:
		return "Failure";
	case DownloadStatusGivingUp:
		return "GivingUp";
	case DownloadStatusComplete:
		return "Complete";
	default:
		return "(Invalid)";
	}
}

bool ofxDownloader::stopDownloadLocked(DownloadInfo &info) {
	if (info._threadId >= 0) {
		ofxLogVer("Download #" << info._downloadId << " currently claimed by thread #" <<
			info._threadId);
		for (std::list<WorkerThread *>::iterator it = _runningThreads.begin();
			it != _runningThreads.end(); it++) {
			WorkerThread *thread = *it;
			if (thread->_threadId == info._threadId) {
				ofxLogVer("Thread #" << thread->_threadId << " found, stopping it");
				info._threadId = -1;
				info._available = false;
				// keep other threads from launching a new thread in order
				// to claim this download
				_unavailable++;
				ofxLogVer(_unavailable << " unavailable download(s) [+]");
				_runningThreads.erase(it);
				ofxLogVer(_runningThreads.size() << " running thread(s) [-]");
				thread->stopThread();
				_mutex.unlock();
				// see stopWorkerThreads()
				thread->getPocoThread().tryJoin(10 * 1000);
				thread->waitForThread();
				_mutex.lock();
				ofxLogVer("Thread #" << thread->_threadId << " stopped");
				delete thread;
				info._available = true;
				_unavailable--;
				ofxLogVer(_unavailable << " unavailable download(s) [-]");
				// don't touch the iterator, it may now be invalid
				goto notClaimed;
			}
		}
		ofxLogErr("Thread #" << info._threadId << " of download #" << info._downloadId <<
			" does not exist");
		return false;
	}
notClaimed:
	ofxLogVer("Download #" << info._downloadId << " not currently claimed");
	for (std::list<DownloadInfo>::iterator it = _pendingDownloads.begin();
		it != _pendingDownloads.end(); it++) {
		if (it->_downloadId == info._downloadId) {
			ofxLogVer("Removing download #" << info._downloadId);
			_pendingDownloads.erase(it);
			ofxLogVer(_pendingDownloads.size() << " pending download(s) [-]");
			return true;
		}
	}
	ofxLogErr("AMAZING! Download #" << info._downloadId << " disappeared!");
	return false;
}

bool ofxDownloader::stopDownload(int32_t id, std::string &infoPath, std::string &dataPath,
	std::string &hashPath) {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Stopping download #" << id);
	if (!_initialized) {
		ofxLogErr("Downloader not initialized");
		return false;
	}
	for (std::list<DownloadInfo>::iterator it = _pendingDownloads.begin();
		it != _pendingDownloads.end(); it++) {
		if (it->_downloadId == id) {
			ofxLogVer("Download #" << id << " found");
			infoPath = it->_infoPath;
			dataPath = it->_dataPath;
			hashPath = it->_hashPath;
			return stopDownloadLocked(*it);
		}
	}
	ofxLogErr("Download #" << id << " does not exist");
	return false;
}

bool ofxDownloader::stopDownload(int32_t id) {
	std::string dummy;
	return stopDownload(id, dummy, dummy, dummy);
}

bool ofxDownloader::cancelDownload(int32_t id) {
	std::string infoPath, dataPath, hashPath;
	if (!stopDownload(id, infoPath, dataPath, hashPath)) {
		ofxLogErr("Error while stopping download #" << id);
		return false;
	}
	bool result = true;
	if (infoPath.size() > 0) {
		ofxLogVer("Removing info file " << infoPath);
		ofFile infoFile(infoPath, ofFile::Reference, true);
		if (infoFile.exists() && !infoFile.remove()) {
			ofxLogErr("Error while removing info file " << infoPath << " of download #" << id);
			result = false;
		}
	}
	if (dataPath.size() > 0) {
		ofxLogVer("Removing data file " << dataPath);
		ofFile dataFile(dataPath, ofFile::Reference, true);
		if (dataFile.exists() && !dataFile.remove()) {
			ofxLogErr("Error while removing data file " << dataPath << " of download #" << id);
			result = false;
		}
	}
	if (hashPath.size() > 0) {
		ofxLogVer("Removing hash file " << hashPath);
		ofFile hashFile(hashPath, ofFile::Reference, true);
		if (hashFile.exists() && !hashFile.remove()) {
			ofxLogErr("Error while removing hash file " << hashPath << " of download #" << id);
			result = false;
		}
	}
	return result;
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
	return true;
}
