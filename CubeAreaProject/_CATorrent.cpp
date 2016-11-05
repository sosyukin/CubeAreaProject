#include "stdafx.h"
#include "_CATorrent.h"



_CATorrent::_CATorrent(_CABencodeDictionaries & bencode)
	: _totalLength(0)
	, _isMultiFiles(false)
{
	GetAnnounce(bencode);
	GetEncoding(bencode);
	GetFileInfo(bencode);
}

_CATorrent::~_CATorrent()
{
}


bool _CATorrent::GetAnnounce(_CABencodeDictionaries & bencode)
{
	if (bencode.Find("announce"))
	{
		_announceList.push_back(_CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionaries["announce"])->_string));
	}
	if (bencode.Find("announce-list"))
	{
		_CABencodeList * pBencodeList = (_CABencodeList *)bencode._dictionaries["announce-list"];
		for (std::list<_CABencode *>::iterator i = pBencodeList->_bencodeList.begin(); i != pBencodeList->_bencodeList.end(); i++)
		{
			_CABencodeList * pBencodeSubList = (_CABencodeList *)*i;
			for (std::list<_CABencode *>::iterator j = pBencodeSubList->_bencodeList.begin(); j != pBencodeSubList->_bencodeList.end(); j++)
			{
				_announceList.push_back(_CACharConversion::ansi2unicode(((_CABencodeString *)*j)->_string));
			}
		}
	}
	return true;
}

bool _CATorrent::GetEncoding(_CABencodeDictionaries & bencode)
{
	if (bencode.Find("encoding"))
	{
		_encoding = _CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionaries["encoding"])->_string);
		return true;
	}
	return false;
}

bool _CATorrent::GetFileInfo(_CABencodeDictionaries & bencode)
{
	if (bencode.Find("info"))
	{
		_CABencodeDictionaries * pFileInfo = (_CABencodeDictionaries *)bencode._dictionaries["info"];
		if (pFileInfo->Find("files"))
		{
			//multi file.
			if (((_CABencodeDictionaries *)pFileInfo->_dictionaries["files"])->GetType() == _CABencode::BencodeType::BenList)
				GetMutliFilesInfo(*(_CABencodeList *)pFileInfo->_dictionaries["files"]);
		}
		else
		{
			//single file.
			GetSingleFileInfo(*pFileInfo);
		}
		if (!pFileInfo->Find("name"))
			return false;
		std::string tstring = ((_CABencodeString *)pFileInfo->_dictionaries["name"])->_string;
		if (pFileInfo->Find("name.utf-8"))
		{
			_name = _CACharConversion::ansi2unicode(tstring);
			_nameUTF8 = ((_CABencodeString *)pFileInfo->_dictionaries["name.utf-8"])->_string;
		}
			
		else if (_encoding.compare(L"UTF-8") == 0)
		{
			_nameUTF8 = tstring;
			_name = _CACharConversion::utf82unicode(tstring);
		}
		else
			_name = _CACharConversion::ansi2unicode(tstring);
		if (!pFileInfo->Find("piece length"))
			return false;
		_pieceLength = ((_CABencodeInteger *)pFileInfo->_dictionaries["piece length"])->_value;
		if (pFileInfo->Find("pieces"))
			if (pFileInfo->_dictionaries["pieces"]->GetType() == _CABencode::BencodeType::BenString)
				return GetPieces(*(_CABencodeString *)pFileInfo->_dictionaries["pieces"]);
		
	}
	return false;
}

bool _CATorrent::GetSingleFileInfo(_CABencodeDictionaries & bencode)
{
	//ExpectFile expectFile;
	if (!bencode.Find("length"))
		return false;
	//expectFile.length = ((_CABencodeInteger *)bencode._dictionaries["length"])->_value;
	_totalLength += ((_CABencodeInteger *)bencode._dictionaries["length"])->_value;

	//if (!bencode.Find("name"))
	//	return false;
	//expectFile.path = _CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionaries["name"])->_string);
	//expectFile.path = expectFile.name;


	
	//_expectFileList.push_back(expectFile);
	return true;
}

bool _CATorrent::GetMutliFilesInfo(_CABencodeList & bencode)
{
	_isMultiFiles = true;
	ExpectFile expectFile;
	for (std::list<_CABencode *>::iterator i = bencode._bencodeList.begin(); i != bencode._bencodeList.end(); i++)
	{
		if ((*i)->GetType() == _CABencode::BencodeType::BenDictionaries)
		{
			_CABencodeDictionaries * pFileBencode = (_CABencodeDictionaries *)(*i);
			if (!pFileBencode->Find("length"))
				return false;
			expectFile.length = ((_CABencodeInteger *)pFileBencode->_dictionaries["length"])->_value;
			_totalLength += expectFile.length;
			if (!pFileBencode->Find("path"))
				return false;
			expectFile.path = GetPath(*(_CABencodeList *)pFileBencode->_dictionaries["path"]);
		}
		_expectFileList.push_back(expectFile);
	}
	return false;
}

bool _CATorrent::GetPieces(_CABencodeString & bencode)
{
	long long piecesNum = bencode._string.length() / 20;
	if (piecesNum * 20 != bencode._string.length())
		return false;
	for (unsigned long long i = 0; i < bencode._string.length(); i+=20)
		_pieceList.push_back(bencode._string.substr(i, i + 20));
	return true;
}

bool _CATorrent::Check(const std::wstring & filePath)
{
	_CAFileStream fileStream(_totalLength);
	std::wstring fullPath(filePath);
	if (fullPath[filePath.length() - 1] != L'\\')
	{
		fullPath.append(L"\\");
	}
	if (_isMultiFiles)
	{
		//MultiFiles
		fullPath.append(_name);
		for (auto i : _expectFileList)
		{
			fileStream.AddFile(std::wstring(fullPath).append(i.path));
		}
	}
	else
	{
		//SingleFile
		fileStream.AddFile(fullPath.append(_name));
	}
	/// TODO : SHA1check
	
	CSHA1 sha1;
	size_t pieceNumber = 0;
	unsigned char shastr[21];
	shastr[20] = '\0';
	for (auto expectedPieceSHA1 : _pieceList)
	{
		pieceNumber++;
		std::string piece = fileStream.Fetch(_pieceLength);
		sha1.Reset();
		sha1.Update((unsigned char *)piece.c_str(), (unsigned int)piece.size());
		sha1.Final();
		sha1.GetHash(shastr);
		if (strncmp(expectedPieceSHA1.c_str(), (char *)shastr, 20) != 0)
		{
			std::wstringstream ss;
			std::wstring pieceInfo;
			ss << pieceNumber;
			std::wstring sst;
			ss >> sst;
			pieceInfo.append(L"( ").append(sst).append(L" / ");
			ss.clear();
			sst.clear();
			ss << _pieceList.size();
			ss >> sst;
			pieceInfo.append(sst).append(L" ) has Error.\n");
			_CACodeLab::FileOut(std::wstring(_name).append(pieceInfo), L"D:\\BTTest\\Log\\PiecesError.txt");
			std::cerr << "( " << pieceNumber << " / " << _pieceList.size() << " ) Has Error." << std::endl;
		}
		else
		{
			std::cerr << "( " << pieceNumber << " / " << _pieceList.size() << " ) is ok." << std::endl;
		}
	}
	return false;
}

std::wstring _CATorrent::GetPath(_CABencodeList & bencode)
{
	std::wstring path;
	for (std::list<_CABencode *>::iterator i = bencode._bencodeList.begin(); i != bencode._bencodeList.end(); i++)
		if ((*i)->GetType() == _CABencode::BencodeType::BenString)
		{
			std::string orgPath(((_CABencodeString *)(*i))->_string);
			if (_encoding.compare(L"UTF-8") == 0)
			{
				path.append(L"\\").append(_CACharConversion::utf82unicode(orgPath));
			}
			else
			{
				path.append(L"\\").append(_CACharConversion::ansi2unicode(orgPath));
			}
			
		}
			
	return path;
}
