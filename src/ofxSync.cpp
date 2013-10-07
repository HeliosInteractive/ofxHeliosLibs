#include "ofxSync.h"

const std::string ofxSync::SyncThread::BOB("bob");
static const std::string FILE_PREFIX("!!!-");
static const std::string INFO_EXTENSION(".dif");
static const std::string DATA_EXTENSION(".ddf");
static const std::string HASH_EXTENSION(".dhf");
static const size_t INFO_LENGTH = FILE_PREFIX.size() + 32 + INFO_EXTENSION.size();
int32_t ofxSync::_refCount = 0;

static const std::string makeInfoFileName(const std::string &suffix) {
	return FILE_PREFIX + suffix + INFO_EXTENSION;
}

static const std::string makeDataFileName(const std::string &suffix) {
	return FILE_PREFIX + suffix + DATA_EXTENSION;
}

static const std::string makeHashFileName(const std::string &suffix) {
	return FILE_PREFIX + suffix + HASH_EXTENSION;
}

static const std::string urlToFileName(const std::string &url) {
	ofxLogVer("Extracting file name from URL " << url);
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
	std::transform(scheme.begin(), scheme.end(), scheme.begin(), ::tolower);
	if (scheme != "http" && scheme != "https") {
		ofxLogErr("URL " << url << " with scheme " << scheme << " is not a HTTP(S) URL");
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

std::string ofxSync::FileInfo::toString() const {
	ostringstream oss;
	oss << "[URL " << _url << ", file name " << _fileName << ", length " << _length <<
		", MD5 digest " << _md5Digest << "]";
	return oss.str();
}

std::string ofxSync::SyncRecord::toString() const {
	ostringstream oss;
	oss << FileInfo::toString() << ", [thread " << _threadId << ", attempt " << _attempt <<
		", resume " << _resume << ", info file " << _infoPath << ", lenient " << _lenient << "]";
	return oss.str();
}

std::string ofxSync::Md5Context::resultToString(const uint8_t md5Result[16]) {
	static char hexDigits[17] = "0123456789abcdef";
	char digestString[32];
	for (int32_t i = 0; i < 16; i++) {
		digestString[2 * i] = hexDigits[md5Result[i] >> 4];
		digestString[2 * i + 1] = hexDigits[md5Result[i] & 15];
	}
	return std::string(digestString, sizeof digestString);
}

bool ofxSync::SyncRecord::writeToFile(const std::string &path) {
	ofxLogVer("Storing sync record in info file " << path);
	ofFile file;
	if (!file.open(path, ofFile::WriteOnly, true)) {
		ofxLogErr("Error while opening info file to store sync record " << toString());
		return false;
	}
	file << _url << std::endl;
	file << _fileName << std::endl;
	file << _length << std::endl;
	file << _md5Digest << std::endl;
	file << _attempt << std::endl;
	file << _resume << std::endl;
	file << _filePath << std::endl;
	file << _infoPath << std::endl;
	file << _dataPath << std::endl;
	file << _hashPath << std::endl;
	file << _lenient << std::endl;
	if (!file.good()) {
		ofxLogErr("Error while writing sync record " << toString());
		return false;
	}
	return true;
}

bool ofxSync::SyncRecord::readFromFile(const std::string &path) {
	ofxLogVer("Loading sync record from info file " << path);
	ofFile file;
	if (!file.open(path, ofFile::ReadOnly, true)) {
		ofxLogErr("Error while opening info file to load sync record " << toString());
		return false;
	}
	std::getline(file, _url);
	std::getline(file, _fileName);
	std::string length;
	std::getline(file, length);
	std::istringstream lengthIss(length);
	if (!(lengthIss >> _length)) {
		ofxLogErr("Invalid length in info file " << path);
		return false;
	}
	std::getline(file, _md5Digest);
	std::string attempt;
	std::getline(file, attempt);
	std::istringstream attemptIss(attempt);
	if (!(attemptIss >> _attempt)) {
		ofxLogErr("Invalid attempt in info file " << path);
		return false;
	}
	std::string resume;
	std::getline(file, resume);
	std::istringstream resumeIss(resume);
	if (!(resumeIss >> _resume)) {
		ofxLogErr("Invalid resume flag in info file " << path);
		return false;
	}
	std::getline(file, _filePath);
	std::getline(file, _infoPath);
	std::getline(file, _dataPath);
	std::getline(file, _hashPath);
	std::string lenient;
	std::getline(file, lenient);
	std::istringstream lenientIss(lenient);
	if (!(lenientIss >> _lenient)) {
		ofxLogErr("Invalid lenient flag in info file " << path);
		return false;
	}
	if (!file.good()) {
		ofxLogErr("Error while reading from info file " << path);
		return false;
	}
	return true;
}

bool ofxSync::Md5Context::readFromFile(const std::string &path) {
	ofxLogVer("Loading MD5 context from " << path);
	ofFile file;
	if (!file.open(path, ofFile::ReadOnly, true)) {
		ofxLogErr("Error while opening hash file " << path << " for reading");
		return false;
	}
	const ofBuffer &buffer = file.readToBuffer();
	if (buffer.size() != sizeof (Md5Context)) {
		ofxLogErr("Unexpected amount of data read from hash file " << path << "(" << buffer.size() <<
			" vs. " << sizeof (Md5Context));
		return false;
	}
	memcpy(this, buffer.getBinaryBuffer(), sizeof (Md5Context));
	return true;
}

bool ofxSync::Md5Context::writeToFile(const std::string &path) {
	ofxLogVer("Storing MD5 context in " << path);
	ofFile file;
	if (!file.open(path, ofFile::WriteOnly, true)) {
		ofxLogErr("Error while opening hash file " << path << " for writing");
		return false;
	}
	if (!file.writeFromBuffer(ofBuffer(reinterpret_cast<const char *>(this),
		sizeof (Md5Context))) || !file.good()) {
		ofxLogErr("Error while writing to hash file " << path);
		return false;
	}
	return true;
}

const char *ofxSync::SyncThread::stateToText(SyncThreadState state) {
	switch (state) {
	case SyncThreadStateClaim:
		return "Claim";
	case SyncThreadStateConnect:
		return "Connect";
	case SyncThreadStateDownload:
		return "Download";
	case SyncThreadStatePersist:
		return "Persist";
	case SyncThreadStateError:
		return "Error";
	case SyncThreadStateComplete:
		return "Complete";
	case SyncThreadStateExit:
		return "Exit";
	default:
		return "(Invalid)";
	}
}

bool removeFiles(const ofxSync::SyncRecord *record, bool file, bool info, bool data, bool hash) {
	bool result = true;
	if (file && record->_filePath.size() > 0 && ofFile::doesFileExist(record->_filePath, false)) {
		ofxLogVer("Removing file " << record->_filePath);
		if (!ofFile::removeFile(record->_filePath, false)) {
			ofxLogErr("Error while removing file " << record->_filePath << " for sync record " <<
				record->_fileName << " with URL " << record->_url);
			result = false;
		}
	}
	if (info && record->_infoPath.size() > 0 && ofFile::doesFileExist(record->_infoPath, false)) {
		ofxLogVer("Removing info file " << record->_infoPath);
		if (!ofFile::removeFile(record->_infoPath, false)) {
			ofxLogErr("Error while removing info file " << record->_infoPath << " for sync record " <<
				record->_fileName << " with URL " << record->_url);
			result = false;
		}
	}
	if (data && record->_dataPath.size() > 0 && ofFile::doesFileExist(record->_dataPath, false)) {
		ofxLogVer("Removing data file " << record->_dataPath);
		if (!ofFile::removeFile(record->_dataPath, false)) {
			ofxLogErr("Error while removing data file " << record->_dataPath << " for sync record " <<
				record->_fileName << " with URL " << record->_url);
			result = false;
		}
	}
	if (hash && record->_hashPath.size() > 0 && ofFile::doesFileExist(record->_hashPath, false)) {
		ofxLogVer("Removing hash file " << record->_hashPath);
		if (!ofFile::removeFile(record->_hashPath, false)) {
			ofxLogErr("Error while removing hash file " << record->_hashPath << " for sync record " <<
				record->_fileName << " with URL " << record->_url);
			result = false;
		}
	}
	return result;
}

bool ofxSync::SyncThread::stateClaim(bool &resume) {
	if ((_record = _sync->claimSyncRecord(_threadId)) == 0)
		return false;
	ofFile dataFile(_record->_dataPath, ofFile::Reference, true);
	ofFile hashFile(_record->_hashPath, ofFile::Reference, true);
	if (dataFile.exists()) {
		ofxLogVer("Data file " << _record->_dataPath << " exists already");
		if (!_record->_resume) {
			ofxLogVer("Removing existing data file");
			if (!dataFile.remove()) {
				ofxLogErr("Error while removing existing data file " << _record->_dataPath <<
					" for sync record " << _record->_fileName);
				return false;
			}
		} else {
			_received = dataFile.getSize();
			ofxLogVer("Keeping existing data file with " << _received << " byte(s)");
		}
	}
	if (_received > 0) {
		ofxLogVer("Looking for hash file " << _record->_hashPath);
		if (!hashFile.exists()) {
			ofxLogErr("Missing hash file " << _record->_hashPath << " for data file " <<
				_record->_dataPath << " with length " << _received << " for sync record " <<
				_record->_fileName);
			return false;
		}
		if (!_md5Context.readFromFile(_record->_hashPath)) {
			ofxLogErr("Error while reading hash file " << _record->_hashPath << " for sync record " <<
				_record->_fileName);
			return false;
		}
	} else if (hashFile.exists()) {
		ofxLogVer("Removing existing hash file " << _record->_hashPath);
		if (!hashFile.remove()) {
			ofxLogErr("Error while removing existing hash file " << _record->_hashPath <<
				" for sync record " << _record->_fileName);
			return false;
		}
	}
	ofxLogVer("Opening data file " << _record->_dataPath);
	_dataFile = new ofFile();
	if (!_dataFile->open(_record->_dataPath, ofFile::Append, true)) {
		ofxLogErr("Error while opening data file " << _record->_dataPath << " for sync record " <<
			_record->_fileName);
		return false;
	}
	ofxLogNot("Thread " << _threadId << " claimed sync record " << _record->_fileName <<
		" with URL " << _record->_url);
	_state = SyncThreadStateConnect;
	if (_sync->_callback != 0)
		_sync->_callback(_sync->_opaque, SyncStatusStarted, _threadId, _received,
		_record->_length, _record->_attempt + 1);
	return true;
}

bool ofxSync::SyncThread::stateConnect(bool &resume) {
	ofxLogVer("Connecting sync record " << _record->_fileName << " to " << _record->_url);
	std::string scheme, host, path;
	int32_t port;
	try {
		Poco::URI uri(_record->_url);
		scheme = uri.getScheme();
		host = uri.getHost();
		path = uri.getPathAndQuery();
		port = uri.getPort();
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while parsing URL " << _record->_url << " for sync record " <<
			_record->_fileName << ": " << ex.name() << " (" << ex.message() << ")");
		return false;
	}
	std::transform(scheme.begin(), scheme.end(), scheme.begin(), ::tolower);
	if (scheme != "http" && scheme != "https") {
		ofxLogErr("Invalid URL scheme " << scheme);
		return false;
	}
	ofxLogVer("Resolving server DNS name " << host);
	Poco::Net::IPAddress ipAddress;
	try {
		ipAddress = Poco::Net::DNS::hostByName(host).addresses()[0];
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while resolving server DNS name " << host << " for sync record " <<
			_record->_fileName << " with URL " << _record->_url << ": " << ex.name() << " (" <<
			ex.message() << ")");
		resume = true;
		return false;
	}
	ofxLogVer("Initializing HTTP(S) session with " << ipAddress.toString() << ":" << port);
	try {
		_session = port != 443 ?
			new Poco::Net::HTTPClientSession() :
			new Poco::Net::HTTPSClientSession();
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while initializing HTTP(S) session: " << ex.name() << " (" << ex.message() <<
			")");
		return false;
	}
	_session->setHost(host);
	_session->setPort(port);
	_session->setKeepAlive(false);
	_session->setTimeout(_sync->_connectTimeout * 1000 * 1000);
	ofxLogVer("Creating HTTP request for " << path);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path,
		Poco::Net::HTTPMessage::HTTP_1_1);
	if (_record->_resume && _received > 0) {
		ofxLogVer("Adding HTTP range header");
		ostringstream oss;
		oss << "bytes=" << _received << "-";
		request.add("Range", oss.str());
	}
	ofxLogVer("Sending HTTP request");
	try {
		_session->sendRequest(request);
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while connecting to " << ipAddress.toString() << ":" << port << " (" <<
			host << ") for sync record " << _record->_fileName << " with URL " << _record->_url <<
			": " << ex.name() << " (" << ex.message() << ")");
		resume = true;
		return false;
	}
	ofxLogVer("Receiving HTTP response");
	Poco::Net::HTTPResponse response;
	try {
		_stream = &_session->receiveResponse(response);
	} catch (Poco::Exception &ex) {
		ofxLogErr("Error while receiving HTTP response from " << host << " for sync record " <<
			_record->_fileName << " with URL " << _record->_url << ": " << ex.name() << " (" <<
			ex.message() << ")");
		resume = true;
		return false;
	}
	switch (response.getStatus()) {
	case Poco::Net::HTTPResponse::HTTP_OK:
		ofxLogVer("Received 200 OK");
		if (_record->_resume && _received > 0) {
			ofxLogErr(host << " failed to resume sync record " << _record->_fileName << " with URL " <<
				_record->_url);
			return false;
		}
		break;
	case Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT:
		ofxLogVer("Received 206 Partial Content");
		if (!_record->_resume || _received == 0) {
			ofxLogErr("Unexpected partial content from " << host << " for sync record " <<
				_record->_fileName << " with URL " << _record->_url);
			return false;
		}
		if (!response.has("Content-Range")) {
			ofxLogErr("Missing Content-Range header from " << host << " for sync record " <<
				_record->_fileName << " with URL " << _record->_url);
			return false;
		}
		{
			ostringstream oss;
			oss << "bytes " << _received << "-";
			const std::string &expected = oss.str();
			const std::string &actual = response.get("Content-Range", "");
			ofxLogVer("Content-Range header is " << actual);
			if (actual.size() <= expected.size() || actual.substr(0, expected.size()) != expected) {
				ofxLogErr("Unexpected Content-Range header from " << host << " for sync record " <<
					_record->_fileName << " with URL " << _record->_fileName << " (" << actual <<
					" vs. " << expected << "*)");
				return false;
			}
		}
		break;
	case Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY:
	case Poco::Net::HTTPResponse::HTTP_FOUND:
	case Poco::Net::HTTPResponse::HTTP_SEE_OTHER:
	case Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT:
		if (!response.has("Location")) {
			ofxLogErr("HTTP " << response.getStatus() << " redirection without location header "
				"from " << host << " for sync record " << _record->_fileName << " with URL " <<
				_record->_url);
			return false;
		}
		ofxLogVer("HTTP " << response.getStatus() << " redirection to " <<
			response["Location"] <<	" from " << host << " for sync record " << _record->_fileName <<
			" with URL " << _record->_url);
		_record->_url = response["Location"];
		// POCO's HTTP session doesn't purge the 3xx response's body,
		// so don't re-use the existing session
		ofxLogVer("Destroying HTTP session");
		delete _session;
		_session = 0;
		// stay in SyncThreadStateConnect and reconnect with new URL
		return true;
	default:
		ofxLogErr("HTTP response with invalid HTTP status code " << response.getStatus() <<
			" from " << host << " for sync record " << _record->_fileName << " with URL " <<
			_record->_url);
		resume = true;
		return false;
	}
	int64_t contentLength = response.getContentLength64();
	if (contentLength != Poco::Net::HTTPResponse::UNKNOWN_CONTENT_LENGTH) {
		ofxLogVer("HTTP response has Content-Length header");
		if (_record->_resume && _received > 0) {
			ofxLogVer("Adjusting content length from " << contentLength << " to " <<
				(contentLength + _received));
			contentLength += _received;
			ostringstream oss;
			oss << "/" << contentLength;
			const std::string &expected = oss.str();
			const std::string &actual = response.get("Content-Range", "");
			if (actual.size() <= expected.size() ||
				actual.substr(actual.size() - expected.size()) != oss.str()) {
				ofxLogErr("Unexpected Content-Range header from " << host << " for sync record " <<
					_record->_fileName << " with URL " << _record->_url << " (" << actual << " vs. *" <<
					expected << ")");
				return false;
			}
		}
		if (_record->_length < 0) {
			ofxLogVer("Accepting content length " << contentLength << " from " << host <<
				" for sync record " << _record->_fileName << " with URL " << _record->_url);
			_record->_length = contentLength;
		} else if (_record->_length != contentLength) {
			ofxLogErr("Content length mismatch for sync record " << _record->_fileName <<
				" with URL " << _record->_url << " (" << contentLength << " vs. " << _record->_length <<
				")");
			return false;
		}
	} else {
		ofxLogVer("HTTP response without Content-Length header");
		if (_record->_lenient)
			ofxLogVer("Leniently accepting HTTP response for sync record " << _record->_fileName <<
				" with URL " << _record->_url);
		else if (_record->_length < 0 && _record->_md5Digest.empty()) {
			ofxLogErr("HTTP response from " << host << " without Content-Length header for "
				" sync record " << _record->_fileName << " with URL " << _record->_url <<
				" without known length or MD5 digest");
			return false;
		}
	}
	_session->socket().setReceiveTimeout(_sync->_transferTimeout * 1000 * 1000);
	_state = SyncThreadStateDownload;
	return true;
}

bool ofxSync::SyncThread::stateDownload(bool &resume) {
	ofxLogVer("Downloading file for sync record " << _record->_fileName << " with URL " <<
		_record->_url);
	// POCO doesn't make a difference between regular EOF and connection
	// or HTTP protocol errors; rely on file size and MD5 digest to detect
	// transmission problems
	if (_stream->good()) {
		_stream->read(_buffer, BUFFER_SIZE);
		streamsize count = _stream->gcount();
		_received += count;
		ofxLogVer("Received " << count << " (total: " << _received << ") byte(s)");
		MD5_Update(&_md5Context, _buffer, count);
		if (!_dataFile->writeFromBuffer(ofBuffer(_buffer, count))) {
			ofxLogErr("Error while writing " << count << " byte(s) to data file " <<
				_record->_dataPath << " for sync record " << _record->_fileName << " with URL " <<
				_record->_url);
			return false;
		}
		if (!_md5Context.writeToFile(_record->_hashPath)) {
			ofxLogErr("Error while updating hash file " << _record->_hashPath << " for sync record " <<
				_record->_fileName << " with URL " << _record->_url);
			return false;
		}
		if (false && _received == 10000) {
			// resume for testing
			resume = true;
			return false;
		}
		if (_sync->_callback != 0)
			_sync->_callback(_sync->_opaque, SyncStatusProgress, _threadId, _received,
			_record->_length, _record->_attempt + 1);
	} else {
		ofxLogVer("End of stream or error for sync record " << _record->_fileName << " with URL " <<
			_record->_url);
		if (_record->_length >= 0 && _received != _record->_length) {
			ofxLogErr("Unexpected received data length for sync record " << _record->_fileName <<
				" with URL " << _record->_url << " (" << _received << " vs. " << _record->_length <<
				")");
			if (_received < _record->_length)
				resume = true;
			return false;
		}
		uint8_t md5Result[16];
		MD5_Final(md5Result, &_md5Context);
		const std::string &digest = Md5Context::resultToString(md5Result);
		if (_record->_md5Digest.size() > 0 && digest != _record->_md5Digest) {
			ofxLogErr("MD5 digest mismatch for sync record " << _record->_fileName << " with URL " <<
				_record->_url << " (" << digest << " vs. " << _record->_md5Digest << ")");
			// don't resume, if the length is correct, but the MD5 digest is wrong
			if (!(_record->_length >= 0 && _received == _record->_length))
				resume = true;
			return false;
		}
		if (_record->_lenient)
			ofxLogVer("Leniently letting sync record " << _record->_fileName << " with URL " <<
				_record->_url << " pass");
		else
			ofxLogVer("Sync record " << _record->_fileName << " with URL " << _record->_url <<
				" passed length and MD5 digest checks");
		_state = SyncThreadStatePersist;
	}
	return true;	
}

bool ofxSync::SyncThread::statePersist(bool &resume) {
	ofxLogVer("Persisting sync record " << _record->_fileName << " with URL " << _record->_url <<
		" to " << _record->_filePath);
	ofxLogVer("Destroying data file");
	delete _dataFile;
	_dataFile = 0;
	if (ofFile::doesFileExist(_record->_filePath, false)) {
		ofxLogErr("AMAZING! File " << _record->_filePath << " exists already for sync record " <<
			_record->_fileName << " with URL " << _record->_url);
		if (!removeFiles(_record, true, false, false, false)) {
			ofxLogErr("Error while removing file");
		}
	}
	if (!ofFile::moveFromTo(_record->_dataPath, _record->_filePath, false, true)) {
		ofxLogErr("Error while renaming data file " << _record->_dataPath << " to file " <<
			_record->_filePath << " sync record " << _record->_fileName << " with URL " <<
			_record->_url);
		return false;
	}
	if (!removeFiles(_record, false, true, false, true)) {
		ofxLogErr("Error while removing info and hash files");
		return false;
	}
	ofxLogVer("Sync record " << _record->_fileName << " with URL " << _record->_url << " persisted");
	_state = SyncThreadStateComplete;
	return true;	
}

void ofxSync::SyncThread::stateError(bool resume) {
	if (_record == 0)
		ofxLogErr("Thread " << _threadId << " failed without a sync record");
	else {
		ofxLogErr("Sync record " << _record->_fileName << " with URL " << _record->_url <<
			" failed on thread " << _threadId);
		if (!resume && _record->_resume)
			_record->_resume = false;
		if (_sync->_callback != 0)
			_sync->_callback(_sync->_opaque, SyncStatusFailure, _threadId, _received, _record->_length,
				_record->_attempt + 1);
		_record->_attempt++;
		if (!_sync->_retry && _dataFile != 0) {
			ofxLogVer("Destroying data file");
			delete _dataFile;
			_dataFile = 0;
		}
		_sync->releaseSyncRecord(_record, !_sync->_retry);
	}
	_state = SyncThreadStateExit;
}

void ofxSync::SyncThread::stateComplete() {
	ofxLogNot("Sync record " << _record->_fileName << " with URL " << _record->_url << " " <<
		"completed on thread " << _threadId);
	_record->_filePath.clear();
	_record->_infoPath.clear();
	_record->_dataPath.clear();
	_record->_hashPath.clear();
	if (_sync->_callback != 0)
		_sync->_callback(_sync->_opaque, SyncStatusComplete, _threadId, _received, _record->_length,
			_record->_attempt + 1);
	_sync->releaseSyncRecord(_record, false);
	_state = SyncThreadStateExit;
}

void ofxSync::SyncThread::threadedFunction() {	
	ofxLogVer("Entering thread " << _threadId);
	_sync->zombieSlayer(false);
	bool resume = false;
	while (isThreadRunning()) {
		ofxLogVer("Thread #" << _threadId << " is in state " << stateToText(_state));
		if (_state == SyncThreadStateExit)
			break;
		switch (_state) {
		case SyncThreadStateClaim:
			if (!stateClaim(resume))
				_state = SyncThreadStateError;
			break;
		case SyncThreadStateConnect:
			if (!stateConnect(resume))
				_state = SyncThreadStateError;
			break;
		case SyncThreadStateDownload:
			if (!stateDownload(resume))
				_state = SyncThreadStateError;
			break;
		case SyncThreadStatePersist:
			if (!statePersist(resume))
				_state = SyncThreadStateError;
			break;
		case SyncThreadStateError:
			stateError(resume);
			break;
		case SyncThreadStateComplete:
			stateComplete();
			break;
		case SyncThreadStateExit:
			ofxLogVer("AMAZING! I reached the exit state");
			break;
		}
	}
	if (_session != 0) {
		ofxLogVer("Destroying HTTP session");
		delete _session;
		_session = 0;
	}
	if (_dataFile != 0) {
		ofxLogVer("Destroying data file");
		delete _dataFile;
		_dataFile = 0;
	}
	ofxScopeMutex m(_sync->_mutex);
	if (isThreadRunning()) {
		_sync->threadDoneLocked(_threadId);
		_sync->startThreadsLocked();
	} else
		ofxLogVer("Not touching thread info during shutdown");
	ERR_remove_thread_state(0);
	ofxLogVer("Leaving thread " << _threadId);
}

ofxSync::SyncRecord *ofxSync::claimSyncRecord(int32_t threadId) {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Thread " << threadId << " trying to claim a sync record");
	for (std::list<SyncRecord *>::iterator it = _recordQueue.begin(); it != _recordQueue.end();
		it++) {
		if ((*it)->_threadId < 0 && !(*it)->_infoPath.empty()) {
			ofxLogVer("Thread " << _threadId << " claimed sync record " << (*it)->_fileName);
			(*it)->_threadId = threadId;
			return *it;
		}
	}
	ofxLogErr("AMAZING! Thread " << threadId << " did not find any sync record to claim");
	return 0;
}

bool ofxSync::releaseSyncRecord(const SyncRecord *record, bool remove) {
	ofxScopeMutex m(_mutex);
	int32_t threadId = record->_threadId;
	ofxLogVer("Thread " << threadId << " releasing sync record " << record->_fileName <<
		" with URL " << record->_url);
	for (std::list<SyncRecord *>::iterator it = _recordQueue.begin(); it != _recordQueue.end();
		it++) {
		if (*it == record) {
			const std::string fileName = record->_fileName, url = record->_url;
			if (remove) {
				ofxLogVer("Removing sync record");
				if (!removeFiles(record, true, true, true, true)) {
					ofxLogErr("Error while removing files for sync record " << record->toString());
				}
				_recordQueue.erase(it);
				if (_recordMap.erase(record->_fileName) < 1) {
					ofxLogErr("Error while removing sync record " << record->toString());
				}
				ofxLogVer("Thread " << threadId << " removed sync record " << fileName <<
					" with URL " << url);
			} else {
				ofxLogVer("Keeping sync record");
				(*it)->_threadId = -1;
				// move to the end of the queue
				_recordQueue.splice(_recordQueue.end(), _recordQueue, it);
				ofxLogVer("Thread " << threadId << " released sync record " << fileName <<
					" with URL " << url);
			}
			return true;
		}
	}
	ofxLogErr("AMAZING! Thread " << threadId << " released non-existent sync record " <<
		record->_fileName << " with URL " << record->_url);
	return false;
}

void ofxSync::startThreadsLocked() {
	int32_t threadCount = static_cast<int32_t>(_livingThreads.size());
	int32_t syncRecordCount = 0;
	for (std::list<SyncRecord *>::iterator it = _recordQueue.begin(); it != _recordQueue.end();
		it++)
		if ((*it)->_threadId < 0 && !(*it)->_infoPath.empty())
			syncRecordCount++;
	ofxLogVer("Starting threads, " << threadCount << " running, maximum is " << _maxThreads <<
		", " << syncRecordCount << " claimable sync record(s)");
	while (threadCount < _maxThreads && threadCount < syncRecordCount) {
		SyncThread *thread = new SyncThread(this, ++_threadId);
		_livingThreads.push_back(thread);
		thread->startThread();
		threadCount++;
		ofxLogVer("Created new thread " << _threadId << ", now " << threadCount << " running");
	}
	ofxLogVer("Threads started");
}

void ofxSync::stopThreadsLocked() {
	ofxLogVer("Stopping threads");
	while (_livingThreads.size() > 0) {
		SyncThread *thread = _livingThreads.front();
		_livingThreads.pop_front();
		ofxLogVer("Stopping thread " << thread->_threadId);
		thread->stopThread();
		_mutex.unlock();
		// waitForThread() doesn't join, if the thread is not running anymore,
		// which leads to a pthread resource leak
		thread->getPocoThread().tryJoin(10 * 1000);
		thread->waitForThread();
		_mutex.lock();
		ofxLogVer("Thread " << thread->_threadId << " stopped");
		delete thread;
	}
	ofxLogVer("Threads stopped");
}

void ofxSync::threadDoneLocked(int32_t threadId) {
	ofxLogVer("Thread " << threadId << " is done");
	for (std::list<SyncThread *>::iterator it = _livingThreads.begin(); it != _livingThreads.end();
		it++) {
		if ((*it)->_threadId == threadId) {
			_zombieThreads.push_back(*it);
			_livingThreads.erase(it);
			ofxLogVer("Thread " << threadId << " now a zombie, " << _livingThreads.size() <<
				" still running");
			return;
		}
	}
	ofxLogErr("AMAZING! Thread " << threadId << " not found");
}

void ofxSync::zombieSlayerLocked(bool shutdown) {
	int32_t min = shutdown ? 0 : _maxThreads;
	ofxLogVer("Slaying zombie threads, minimum is " << min);
	while (static_cast<int32_t>(_zombieThreads.size()) > min) {
		SyncThread *thread = _zombieThreads.front();
		_zombieThreads.pop_front();
		ofxLogVer("Slaying zombie thread " << thread->_threadId);
		// see stopThreadsLocked()
		thread->getPocoThread().tryJoin(10 * 1000);
		thread->waitForThread();
		ofxLogVer("Thread " << thread->_threadId << " now dead");
		delete thread;
	}
	ofxLogVer("Done slaying zombie threads");
}

void ofxSync::zombieSlayer(bool shutdown) {
	ofxScopeMutex m(_mutex);
	zombieSlayerLocked(shutdown);
}

bool ofxSync::initialize(const std::string &cacheDir) {
	ofxLogVer("Initializing");
	assert(sizeof (MD5_CTX) == sizeof (Md5Context));
	if (_init) {
		ofxLogErr("Already initialized");
		return false;
	}
	if (++_refCount == 1) {
		ofxLogVer("Initializing SSL subsystem");
		try {
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
				"", Poco::Net::Context::VERIFY_RELAXED, 9, true);
			Poco::Net::SSLManager::instance().initializeClient(0, 0, context);
		} catch (Poco::Exception &ex) {
			ofxLogErr("Error while initializing SSL subsystem: " << ex.name() << " (" <<
				ex.message() << ")");
			return false;
		}
	}
	ofDirectory dir(cacheDir);
	if (!dir.exists()) {
		ofxLogErr("Cache directory " << cacheDir << " does not exist");
		return false;
	}
	if (!dir.isDirectory()) {
		ofxLogErr("Cache directory " << cacheDir << " is not a directory");
		return false;
	}
	_cacheDir = cacheDir;
	_recordMap.clear();
	_recordQueue.clear();
	_livingThreads.clear();
	_zombieThreads.clear();
	int32_t count = dir.listDir();
	bool result = true;
	ofxLogVer("Found " << count << " file(s) in " << _cacheDir);
	for (int32_t i = 0; i < count; i++) {
		const std::string &path = dir.getPath(i);
		if (path.size() > INFO_LENGTH && path.substr(path.size() - INFO_LENGTH,
			FILE_PREFIX.size()) == FILE_PREFIX) {
			if (path.size() > INFO_EXTENSION.size() &&
				path.substr(path.size() - INFO_EXTENSION.size()) == INFO_EXTENSION) {
				ofxLogVer("Processing info file " << path);
				SyncRecord record;
				if (!record.readFromFile(path)) {
					ofxLogErr("Error while reading sync record from info file " << path);
					result = false;
					continue;
				}
				const RecordMapResult &res = _recordMap.insert(RecordMapElement(record._fileName,
					record));
				if (!res.second) {
					ofxLogErr("Duplicate sync record " << record.toString());
					result = false;
					continue;
				}
				_recordQueue.push_back(&res.first->second);
				ofxLogVer("Added sync record from info file: " << res.first->second.toString());
				continue;
			} else if (path.size() > DATA_EXTENSION.size() &&
				path.substr(path.size() - DATA_EXTENSION.size()) == DATA_EXTENSION) {
				ofxLogVer("Skipping data file " << path);
				continue;
			} else if (path.size() > HASH_EXTENSION.size() &&
				path.substr(path.size() - HASH_EXTENSION.size()) == HASH_EXTENSION) {
				ofxLogVer("Skipping hash file " << path);
				continue;
			}
		}
		ofxLogVer("Processing file " << path);
		SyncRecord record(dir.getName(i), path);
		const RecordMapResult &res = _recordMap.insert(RecordMapElement(record._fileName, record));
		if (!res.second) {
			ofxLogErr("Duplicate sync record " << record.toString());
			result = false;
			continue;
		}
		_recordQueue.push_back(&res.first->second);
		ofxLogVer("Added sync record for file: " << res.first->second.toString());
	}
	_init = true;
	return result;
}

const char *ofxSync::statusToText(SyncStatus status) {
	switch (status) {
	case SyncStatusStarted:
		return "Started";
	case SyncStatusProgress:
		return "Progress";
	case SyncStatusFailure:
		return "Failure";
	case SyncStatusComplete:
		return "Complete";
	default:
		return "(Invalid)";
	}
}

void ofxSync::setMaxThreads(int32_t maxThreads) {
	_maxThreads = maxThreads;
}

void ofxSync::setTimeouts(int32_t connectTimeout, int32_t transferTimeout) {
	_connectTimeout = connectTimeout;
	_transferTimeout = transferTimeout;
}

void ofxSync::setRemove(bool remove) {
	_remove = remove;
}

void ofxSync::setRetry(bool retry) {
	_retry = retry;
}

void ofxSync::setCallback(SyncCallback callback, void *opaque) {
	_callback = callback;
	_opaque = opaque;
}
bool ofxSync::synchronize(FileList files, bool lenient) {
	ofxScopeMutex m(_mutex);
	ofxLogVer("Synchronizing " << files.size() << " file(s), " << _recordMap.size() <<
		" currently cached");
	if (!_init) {
		ofxLogErr("Not initialized");
		return false;
	}
	int32_t added = 0, removed = 0, skipped = 0, failed = 0;
	bool missing = false;
	FileMap tmpMap;
	FileList::iterator fit;
	for (fit = files.begin(); fit != files.end(); fit++) {
		if (fit->_url.size() == 0) {
			ofxLogErr("Missing URL in file info " << fit->toString());
			failed++;
			continue;
		}
		if (fit->_fileName.size() == 0)
			fit->_fileName = urlToFileName(fit->_url);
		if (fit->_fileName.size() == 0) {
			ofxLogErr("Invalid URL in file info " << fit->toString());
			failed++;
			continue;
		}
		if (_recordMap.find(fit->_fileName) == _recordMap.end()) {
			ofxLogVer(fit->_fileName << " not found");
			missing = true;
			continue;
		}
	}
	if (!missing && (!_remove || files.size() == _recordMap.size())) {
		ofxLogVer("No changes, not synchronizing");
		startThreadsLocked();
		return true;
	}
	stopThreadsLocked();
	ofxLogVer("Adding file(s)");
	for (fit = files.begin(); fit != files.end(); fit++) {
		if (fit->_url.size() == 0 || fit->_fileName.size() == 0) {
			ofxLogVer("Skipping invalid file info " << fit->toString());
			continue;
		}
		if (fit->_fileName.size() == 0)
			fit->_fileName = urlToFileName(fit->_url);
		if (fit->_fileName.size() == 0) {
			ofxLogErr("Invalid URL in file info " << fit->toString());
			failed++;
			continue;
		}
		std::transform(fit->_md5Digest.begin(), fit->_md5Digest.end(), fit->_md5Digest.begin(),
			::tolower);
		tmpMap.insert(FileMapElement(fit->_fileName, *fit));
		RecordMap::iterator rit;
		if ((rit = _recordMap.find(fit->_fileName)) != _recordMap.end()) {
			ofxLogVer("Skipping file info with existing sync record " << rit->second.toString());
			skipped++;
			continue;
		}
		ofxLogVer("Adding new sync record for " << fit->_fileName);
		Md5Context md5Context;
		MD5_Init(&md5Context);
		MD5_Update(&md5Context, &_startTime, sizeof _startTime);
		MD5_Update(&md5Context, &_threadId, sizeof _threadId);
		MD5_Update(&md5Context, fit->_url.data(), fit->_url.size());
		MD5_Update(&md5Context, fit->_fileName.data(), fit->_fileName.size());
		uint8_t md5Result[16];
		MD5_Final(md5Result, &md5Context);
		const std::string &suffix = Md5Context::resultToString(md5Result);
		SyncRecord record(*fit, lenient);
		record._filePath = ofFilePath::join(_cacheDir, fit->_fileName);
		if (ofFile::doesFileExist(record._filePath, false)) {
			ofxLogErr("AMAZING! File " << record._filePath << " already exists");
			failed++;
			continue;
		}
		record._infoPath = ofFilePath::join(_cacheDir, makeInfoFileName(suffix));
		if (ofFile::doesFileExist(record._infoPath, false)) {
			ofxLogErr("AMAZING! Info file " << record._infoPath << " already exists");
			failed++;
			continue;
		}
		record._dataPath = ofFilePath::join(_cacheDir, makeDataFileName(suffix));
		if (ofFile::doesFileExist(record._dataPath, false)) {
			ofxLogErr("AMAZING! Data file " << record._dataPath << " already exists");
			failed++;
			continue;
		}
		record._hashPath = ofFilePath::join(_cacheDir, makeHashFileName(suffix));
		if (ofFile::doesFileExist(record._hashPath, false)) {
			ofxLogErr("AMAZING! Hash file " << record._hashPath << " already exists");
			failed++;
			continue;
		}
		if (!record.writeToFile(record._infoPath)) {
			ofxLogErr("Error while writing info file for sync record " << record.toString());
			failed++;
			continue;
		}
		const RecordMapResult &res = _recordMap.insert(RecordMapElement(record._fileName, record));
		_recordQueue.push_back(&res.first->second);
		added++;
		ofxLogVer("Added sync record " << res.first->second.toString());
	}
	if (_remove) {
		ofxLogVer("Removing file(s)");
		RecordList::iterator curr, rit = _recordQueue.begin();
		SyncRecord *record;
		while (rit != _recordQueue.end()) {
			curr = rit++;
			record = *curr;
			if (tmpMap.find(record->_fileName) == tmpMap.end()) {
				ofxLogVer("Removing existing sync record " << record->toString());
				if (!removeFiles(record, true, true, true, true)) {
					ofxLogErr("Error while removing files for sync record " << record->toString());
					failed++;
				}
				_recordQueue.erase(curr);
				if (_recordMap.erase(record->_fileName) < 1) {
					ofxLogErr("Error while removing sync record " << record->toString());
					failed++;
				}
				removed++;
			}
		}
	}
	ofxLogVer("Added " << added << " file(s), removed " << removed << ", " << skipped <<
		" skipped, " << failed << " failed");
	startThreadsLocked();
	return failed == 0;
}

bool ofxSync::shutDown() {
	ofxLogVer("Shutting down");
	if (!_init) {
		ofxLogErr("Not initialized");
		return false;
	}
	ofxScopeMutex m(_mutex);
	stopThreadsLocked();
	zombieSlayerLocked(true);
	// otherwise POCO's OpenSSLInitializer's mutex may be destructed
	// before Context, leading to an exception on program exit
	if (--_refCount == 0) {
		ofxLogVer("Shutting down SSL subsystem");
		try {
			Poco::Net::SSLManager::instance().shutdown();
		} catch (Poco::Exception &ex) {
			ofxLogErr("Error while shutting down SSL subsystem: " << ex.name() << " (" <<
				ex.message() << ")");
		}
		ENGINE_cleanup();
		CONF_modules_free();
		COMP_zlib_cleanup();
		CRYPTO_cleanup_all_ex_data();
	}
	_init = false;
	return true;
}
