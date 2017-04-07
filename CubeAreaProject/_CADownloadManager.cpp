#include "stdafx.h"
#include "_CADownloadManager.h"


_CADownloadManager::_CADownloadManager()
{
	if (!_downloadFolder.Open(L"E:\\����"))
	{
		throw std::exception("[ERROR] Download folder open failed.");
	}
	if (!_btFolder.Open(L"E:\\BT"))
	{
		throw std::exception("[ERROR] BT folder open failed.");
	}
	if (!_checkedDownloadFolder.Open(L"E:\\ͨ��\\����"))
	{
		throw std::exception("[ERROR] Checked download folder open failed.");
	}
	if (!_checkedBtFolder.Open(L"E:\\ͨ��\\BT"))
	{
		throw std::exception("[ERROR] Checked BT folder open failed.");
	}
	if (!_errCheckedDownloadFolder.Open(L"E:\\δͨ��\\����"))
	{
		throw std::exception("[ERROR] Error checked download folder open failed.");
	}
	if (!_errCheckedBtFolder.Open(L"E:\\δͨ��\\BT"))
	{
		throw std::exception("[ERROR] Error checked BT folder open failed.");
	}
	if (!_downloadFileNotExistBtFolder.Open(L"E:\\δͨ��\\BTNotExist"))
	{
		throw std::exception("[ERROR] Download file not exist BT folder open failed.");
	}
	if (!_missingFileBtFolder.Open(L"E:\\δͨ��\\BTMissingFile"))
	{
		throw std::exception("[ERROR] Miss file BT folder open failed.");
	}
	if (!_missingFileDownloadFolder.Open(L"E:\\δͨ��\\����MissingFile"))
	{
		throw std::exception("[ERROR] Miss file Download folder open failed.");
	}
	if (!_unableToVerifyBtFolder.Open(L"E:\\δͨ��\\UnableToVerifyBt"))
	{
		throw std::exception("[ERROR] Miss file Download folder open failed.");
	}
	if (!_unableToVerifyDownloadFolder.Open(L"E:\\δͨ��\\UnableToVerifyDownload"))
	{
		throw std::exception("[ERROR] Miss file Download folder open failed.");
	}
}


_CADownloadManager::~_CADownloadManager()
{
}


// Check torrent name and rename
bool _CADownloadManager::TorrentRename()
{
	_btFolder.Scan();
	std::vector<_CAFile *> torrentList;
	_btFolder.GetFileList(L"torrent", torrentList, _CACodeLab::SameStr, _CAFile::ATTR::SUFFIX);
	for (auto torrentFile : torrentList)
	{
		try
		{
			_CATorrent torrent(torrentFile->Path());
			torrent.Rename();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << _CACharConversion::unicode2ansi(torrentFile->Name()) << " can't rename." << std::endl;
		}
	}
	return true;
}


// Check download file throw torrent
bool _CADownloadManager::TorrentDownloadCheck()
{
	_btFolder.Scan();
	std::vector<_CAFile *> torrentList;
	_btFolder.GetFileList(L"torrent", torrentList, _CACodeLab::SameStr, _CAFile::ATTR::SUFFIX);
	for (auto torrentFile : torrentList)
	{
		_CATorrent torrent(torrentFile->Path());
		if (torrent.NoError() && !torrent.HavePaddingFiles())
		{
			try
			{
				if (torrent.Check(_downloadFolder.Path()))
				{
					_CACodeLab::CAMoveFile(torrentFile->Path(), _checkedBtFolder.Path());
					_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _checkedDownloadFolder.Path());
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
				if (0 == strcmp(e.what(), "File not exist."))
				{
					_CACodeLab::CAMoveFile(torrentFile->Path(), _downloadFileNotExistBtFolder.Path());
				}
				else if (0 == strcmp(e.what(), "File missing."))
				{
					_CACodeLab::CAMoveFile(torrentFile->Path(), _missingFileBtFolder.Path());
					_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _missingFileDownloadFolder.Path());
				}
				else
				{
					_CACodeLab::CAMoveFile(torrentFile->Path(), _errCheckedBtFolder.Path());
					_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _errCheckedDownloadFolder.Path());
				}
			}
		}
		else if (torrent.HavePaddingFiles())
		{
			
			//_CACodeLab::CAMoveFile(torrentFile->Path(), _unableToVerifyBtFolder.Path());
			//_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _unableToVerifyDownloadFolder.Path());
			
		}
		
		
	}
	return false;
}
