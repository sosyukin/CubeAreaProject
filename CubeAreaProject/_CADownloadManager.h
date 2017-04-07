#pragma once
class _CADownloadManager
{
private:
	_CAFolder _downloadFolder;
	_CAFolder _btFolder;
	_CAFolder _checkedDownloadFolder;
	_CAFolder _checkedBtFolder;
	_CAFolder _errCheckedDownloadFolder;
	_CAFolder _errCheckedBtFolder;
	_CAFolder _downloadFileNotExistBtFolder;
	_CAFolder _missingFileBtFolder;
	_CAFolder _missingFileDownloadFolder;
	_CAFolder _unableToVerifyBtFolder;
	_CAFolder _unableToVerifyDownloadFolder;
public:
	_CADownloadManager();
	~_CADownloadManager();
	// Check torrent name and rename
	bool TorrentRename();
	// Check download file throw torrent
	bool TorrentDownloadCheck();
};

