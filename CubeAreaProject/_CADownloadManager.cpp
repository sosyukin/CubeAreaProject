#include "stdafx.h"
#include "_CADownloadManager.h"


_CADownloadManager::_CADownloadManager()
{
	if (!_downloadFolder.Open(L"D:\\��У��\\����\\����Ŀ¼"))
	{
		throw std::exception("[ERROR] Download folder open failed.");
	}
	if (!_btFolder.Open(L"D:\\��У��\\����\\BTĿ¼"))
	{
		throw std::exception("[ERROR] BT folder open failed.");
	}
	if (!_checkedDownloadFolder.Open(L"D:\\��У��\\����\\���\\����"))
	{
		throw std::exception("[ERROR] BT folder open failed.");
	}
	if (!_checkedBtFolder.Open(L"D:\\��У��\\����\\���\\BT"))
	{
		throw std::exception("[ERROR] Checked BT folder open failed.");
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
		try
		{
			_CATorrent torrent(torrentFile->Path());
			if (torrent.Check(_downloadFolder.Path()))
			{
				_CACodeLab::CAMoveFile(torrentFile->Path(), _checkedBtFolder.Path());
				_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), _checkedDownloadFolder.Path());
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	return false;
}
