#pragma once
#include "_CAFile.h"
#include "_CABencodeDictionaries.h"
class _CATorrent
{
	struct ExpectFile
	{
		long long length;
		//std::wstring name;
		std::wstring path;
	};
public:
	_CATorrent(_CABencodeDictionaries & bencode);
	~_CATorrent();
	bool GetAnnounce(_CABencodeDictionaries & bencode);
	bool GetEncoding(_CABencodeDictionaries & bencode);
	bool GetFileInfo(_CABencodeDictionaries & bencode);
	bool GetSingleFileInfo(_CABencodeDictionaries & bencode);
	bool GetMutliFilesInfo(_CABencodeList & bencode);
	bool GetPieces(_CABencodeString & bencode);
	std::wstring GetPath(_CABencodeList & bencode);
private:
	std::list<std::wstring> _announceList;
	std::list<ExpectFile> _expectFileList;
	std::vector<std::string> _pieceList;
	std::wstring _encoding;
	std::wstring _name;
	std::string _nameUTF8;
	long long _totalLength;
	long long _pieceLength;
	bool _isMultiFiles;
};

