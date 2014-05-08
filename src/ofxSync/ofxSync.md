# ofxSync

&copy; 2013 Helios Interactive Technologies, Inc.

ofxSync synchronizes a local directory with content from a remote web server. Essentially, you give ofxSync a set of URLs and ofxSync downloads the content from each URL to a file in a directory of your choice. It makes sure that it retries a download later in case of a server or network problem and it supports concurrent downloads on multiple threads, resuming downloads, and MD5 checksums.

It does not, however, detect content changes on the web server. Once ofxSync has downloaded the content for a given URL, it will not revisit that URL again. Maybe it should have been called "ofxDownloader".

## Required Information

For each download you provide the following information to ofxSync.

* **URL**. The URL of the content to be downloaded, e.g., `http://www.test.com/a/b/c/blah.txt?foo=bar`.
* **File Name**. Optional. The file name to be used for storing the content downloaded from the URL. Defaults to the last part of the path component of the URL, e.g., `blah.txt` in the case of `http://www.test.com/a/b/c/blah.txt?foo=bar`.
* **Content Length**. Optional. The length of the content at the URL. Defaults to the length returned by the web server in the `Content-Length` header.
* **MD5 Hash**. Optional. The MD5 checksum of the content at the URL. Used to check the integrity of the downloaded content. If not specified, the integrity check is skipped.

ofxSync uses the HTTP implementation provided by the POCO library that comes with openFrameworks. This implementation doesn't allow us to distinguish between (a) a complete download and (b) a network error. As a workaround, we only consider a download complete, if:

* You provided a content length for the download and ofxSync received that many bytes from the web server.

 **-- or --**

* The web server sent a `Content-Length` header in its response and ofxSync received that many bytes from the web server.

 **-- or --**

* You provided an MD5 checksum for the download and the MD5 checksum of the content received so far matches the MD5 checksum that you provided.

This means that ofxSync will not work for you, if:

* You don't know the length of the download and thus cannot provide a content length to ofxSync.

 **-- and --**

* The web server doesn't send a `Content-Length` header for the download, i.e., it uses `chunked` transfer encoding without a `Content-Length` header.

 **-- and --**

* You don't know the MD5 checksum of the download and thus cannot provide it to ofxSync.

## Temporary Files

For each download, ofxSync creates a temporary random 32-character identifier and, based on that identifier, up to three files:

* The **download info file**, named `!!!-[...].dif` (where `[...]` is the 32-character identifier), which stores meta information about the download, e.g., the information that you provided for this download, URL, content length, ... It is a simple ASCII file with one piece of information per line. Persisting this information in a file allows downloads to be resumed when the program that uses ofxSync restarts after, say, a crash.
* The **download data file**, named `!!!-[...].ddf`, which stores the data downloaded so far. When the download is complete, this file is renamed to the file name that you provided. In this way, a file with your chosen file name atomically appears in the download directory once the download is complete.
* The **download hash file**, named `!!!-[...].dhf`, which stores the internal state of the MD5 digest engine for the data contained in the download data file. ofxSync calculates the MD5 checksum of the downloaded data on the fly as it writes each block of received data to the download data file. In order to be able to resume a download, the MD5 state is persisted after each block of received data. This allows us to efficiently resume downloads by simply reading this file instead of passing the full download data file through MD5 again.


## Download Process

1. You pass the information for all your downloads to ofxSync.
2. ofxSync creates download info files for all your downloads and adds your downloads to its queue.
3. A worker thread picks up the first download in the queue.
4. The worker thread connects to the remote server.
5. When the connection is established, the download data file and the download hash file are created.
6. When POCO's HTTP implementation signals the end of the connection, ofxSync checks whether the download is complete. As explained above, this is done by way of either the content length or the MD5 checksum.
7. If the download is not complete, the download is moved to the end of the queue, so that it gets retried later, after all other downloads had a chance to run.
8. If the download is complete, it is removed from the queue.

Note that the above is slightly simplified. There is typically more than one worker thread, so downloads happen concurrently.

## API

The information about each download is encapsulated in the `ofxSync::FileInfo` class. Its constructor takes the URL as well as optionally the file name, the content length, and the MD5 checksum:

`ofxSync::FileInfo::FileInfo(const std::string &url, const std::string &fileName = "", int64_t length = -1, const std::string &md5Digest = "")`

Downloads are passed to ofxSync as a `std::list` of `ofxSync::FileInfo`, which is available as `ofxSync::FileList`:

`typedef std::list<FileInfo> FileList`

ofxSync offers the following API methods.

`bool initialize(const std::string &cacheDir)`

This method initializes the ofxSync object's queue and sets its download directory to `cacheDir`. It then goes through the download directory and collects all the download info files. Downloads for these download info files are added to ofxSync's queue, worker threads are spun up, and the downloads are resumed.

`bool synchronize(FileList files)`

This passes a list of files to be downloaded to ofxSync. Download info files are created for the downloads, the downloads are added to ofxSync's queue, and worker threads are spun up (if required).

What this method does further depends on the `remove` setting. (See the `setRemove()` method further down.) If removal is enabled, then `synchronize()` removes all downloads from its queue (and the corresponding download info, data, and hash files) that are not in the given list of files. So, when synchronization is complete, you'll have only the files in the list of files in your download directory. Previously completed downloads are deleted, downloads currently in the queue are canceled, and ofxSync **starts over from scratch** using only the files that your provided in your list of files.

Conversely, if removal is disabled, the currently queued downloads are kept and the files passed to `synchronize()` are just added to the end of the queue. The files in the given list of files are downloaded **on top of any downloads** that were completed earlier (and are now in the download directory) or that are currently in the queue.

`bool shutDown()`

This stops all currently running worker threads and shuts down ofxSync's queue. This must be the very last method that you call on an ofxSync object.

`void setMaxThreads(int32_t maxThreads)`

This sets the maximal number of worker threads, i.e., the maximal number of concurrent downloads allowed, to `maxThreads`.

`void setTimeouts(int32_t connectTimeout, int32_t transferTimeout)`

This sets the network timeouts for connecting to the web server to `connectTimeout` and for receiving data from the web server to `transferTimeout`. Both values are given in seconds.

`void setRemove(bool remove)`

This tells ofxSync whether you want removal enabled (`true`) or disabled (`false`). (See the `synchronize()` method above.)

`void setCallback(SyncCallback callback, void *opaque)`

If you want the worker threads to keep you informed about the progress made with each download, use this method to specify a callback function. The callback function must match the following type:

`typedef void (*SyncCallback)(void *opaque, SyncStatus status, int32_t id, const std::string &fileName, int64_t thisLength, int64_t thisTotal, int64_t allLeft, int64_t allTotal, int32_t countLeft, int32_t countTotal, int32_t attempt)`

The `opaque` parameter is simply the `opaque` parameter from the call to `setCallback()` passed back to your callback. `status` is the status of the download. Its definition looks as follows:

	enum SyncStatus {
		SyncStatusStarted,
		SyncStatusProgress,
		SyncStatusFailure,
		SyncStatusComplete,
		SyncStatusInterrupted
	}

Here's what the different values mean:

* `SyncStatusStarted`: A worker thread picked up the download from the queue.
* `SyncStatusProgress`: A data block was received from the web server.
* `SyncStatusFailure`: The download failed and was moved to the end of the queue.
* `SyncStatusComplete`: The download finished successfully.
* `SyncStatusInterruped`: The download was aborted, because you called `shutDown()`.

The order in which you see these values for a single download is always:

1. `SyncStatusStarted`
2. Zero or more times `SyncStatusProgress`
3. Lastly, `SyncStatusFailure` or `SyncStatusComplete` or `SyncStatusInterrupted`

The `id` parameter passed to the callback function is a unique identifier for a download. You'll see any ID value first with a `SyncStatusStarted` status. So, the `SyncStatusStarted` status introduces a new ID to you. After that, subsequent `SyncStatusProgress`, etc. updates for the same download will come with the same ID.

The `fileName` parameter tells you the file name of the download, i.e., either the file name that you specified or, by default, the last part of the path component of the URL. (See above.)

`thisLength` and `thisTotal` give you the amount of downloaded data so far and the total length of the download, so that you can, for example, display a progress bar for an individual download.

`allLeft` and `allTotal` gives you the amount of data remaining to be downloaded in the queue as well as the total amount of data in the queue. `countLeft` and `countTotal` give you the number of files remaining to be downloaded in the queue as well as the total number of files downloaded so far and to be downloaded.

The `attempt` parameter tells you how often ofxSync has retried this download.

## A Note on Multi-Threading

Be careful when using callbacks. They'll be invoked on a worker thread. So, if you update your UI from inside the callback, use a mutex to ensure that your main application thread is not updating the UI at the same time, e.g., in the app's `update()` or `guiEvent()` method.

## Suggested Usage

1. Instantiate an ofxSync object.
2. Configure it by invoking the `set*()` methods.
3. Invoke `initialize()` to specify the download directory and to resume the downloads for the download info files in this directory.
4. Invoke `synchronize()` with different lists of files as often as you like. It's perfectly safe to call this method while worker threads are running. It's all taken care of.
5. When your program exits, call `shutDown()` to gracefully bring down any running worker threads and clear the queue.
