#pragma once
#include "stdafx.h"
class _CATorrent
{
	struct ExpectFile
	{
		long long length;
		std::wstring path;
		std::string fileHash;
	};
public:
	_CATorrent(const std::wstring & torrentPath);
	_CATorrent(_CABencodeDictionary & bencode);
	~_CATorrent();
	bool GetAnnounce(_CABencodeDictionary & bencode);
	bool GetEncoding(_CABencodeDictionary & bencode);
	bool GetFileInfo(_CABencodeDictionary & bencode);
	bool GetSingleFileInfo(_CABencodeDictionary & bencode);
	bool GetMutliFilesInfo(_CABencodeList & bencode);
	bool GetPieces(_CABencodeString & bencode);
	bool Check(const std::wstring & filePath);

	bool Rename();

	bool NoError();
	bool HavePaddingFiles();

	std::wstring GetPath(_CABencodeList & bencode);
	std::wstring GetDownloadPath();


private:
	_CABencodeDictionary _content;
	_CAFile _torrentFile;
	std::list<std::wstring> _announceList;
	std::list<ExpectFile> _expectFileList;
	std::vector<std::string> _pieceList;
	std::wstring _filePath;
	std::wstring _encoding;
	std::wstring _name;
	std::string _nameUTF8;
	long long _totalLength;
	long long _pieceLength;
	bool _isMultiFiles;
	bool _havePaddingFiles;
	bool _haveError;
};

