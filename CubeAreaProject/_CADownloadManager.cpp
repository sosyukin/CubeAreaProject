#include "stdafx.h"
#include "_CADownloadManager.h"


_CADownloadManager::_CADownloadManager(const std::wstring & dirPath)
{
	std::wstring dirPathRoot(dirPath);
	if (!_downloadFolder.Open(dirPathRoot.append(L"\\ÏÂÔØ")))
	{
		throw std::exception("[ERROR] Download folder open failed.");
	}
	if (!_btFolder.Open(dirPathRoot.append(L"\\BTÖÖ×Ó")))
	{
		throw std::exception("[ERROR] BT folder open failed.");
	}
	if (!_checkedDownloadFolder.Open(dirPathRoot.append(L"\\¹ý¼ì")))
	{
		throw std::exception("[ERROR] Checked download folder open failed.");
	}
	if (!_checkedBtFolder.Open(dirPathRoot.append(L"\\¹ý¼ìBT")))
	{
		throw std::exception("[ERROR] Checked BT folder open failed.");
	}
	if (!_errCheckedDownloadFolder.Open(dirPathRoot.append(L"\\¿ÛÁô")))
	{
		throw std::exception("[ERROR] Error checked download folder open failed.");
	}
	if (!_errCheckedBtFolder.Open(dirPathRoot.append(L"\\¿ÛÁôBT")))
	{
		throw std::exception("[ERROR] Error checked BT folder open failed.");
	}
	if (!_downloadFileNotExistBtFolder.Open(dirPathRoot.append(L"\\Î´ÏÂBT")))
	{
		throw std::exception("[ERROR] Download file not exist BT folder open failed.");
	}
	if (!_missingFileBtFolder.Open(dirPathRoot.append(L"\\¶ªÊ§BT")))
	{
		throw std::exception("[ERROR] Miss file BT folder open failed.");
	}
	if (!_missingFileDownloadFolder.Open(dirPathRoot.append(L"\\¶ªÊ§")))
	{
		throw std::exception("[ERROR] Miss file Download folder open failed.");
	}
	if (!_unableToVerifyBtFolder.Open(dirPathRoot.append(L"\\³¬È¨BT")))
	{
		throw std::exception("[ERROR] Miss file Download folder open failed.");
	}
	if (!_unableToVerifyDownloadFolder.Open(dirPathRoot.append(L"\\³¬È¨")))
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
			
			_CACodeLab::CAMoveFile(torrentFile->Path(), _unableToVerifyBtFolder.Path());
			_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _unableToVerifyDownloadFolder.Path());
			
		}
		
		
	}
	return false;
}
