#pragma once
class _CADownloadManager
{
private:
	_CAFolder _downloadFolder;
	_CAFolder _btFolder;
	_CAFolder _checkedDownloadFolder;
	_CAFolder _checkedBtFolder;
public:
	_CADownloadManager();
	~_CADownloadManager();
	// Check torrent name and rename
	bool TorrentRename();
	// Check download file throw torrent
	bool TorrentDownloadCheck();
};

