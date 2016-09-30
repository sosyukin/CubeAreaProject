#include "stdafx.h"
#include "_CATorrent.h"



_CATorrent::_CATorrent(_CABencodeDictionaries & bencode)
	: _totalLength(0)
	, _isMultiFiles(false)
{
	GetAnnounce(bencode);
	GetEncoding(bencode);
	GetFileInfo(bencode);
	//std::map<std::string, _CABencode *>::iterator i = bencode._dictionaries.
	/*std::map<std::string, _CABencode *>::iterator i = bencode._dictionaries.find("announcea");
	_CABencode * tben = bencode._dictionaries["announcea"];
	if (i == bencode._dictionaries.end())
	{
		std::cout << "no i" << std::endl;
	}
	else
		std::cout << i->first << std::endl;*/
	//_announceList.push_back(((_CABencodeString *)bencode._dictionaries["announce"])->_string);
	//for ((bencode._dictionaries["announce-list"] i = 0; i < length; i++)
	//{

	//}
	//if (bencode._dictionaries["announce"]->GetType() == _CABencode::BencodeType::BenString)
	//{
	//	std::cout << "Checked" << std::endl;
	//}
	
}

_CATorrent::~_CATorrent()
{
}


bool _CATorrent::GetAnnounce(_CABencodeDictionaries & bencode)
{
	if (bencode.Find("announce"))
	{
		_announceList.push_back(_CACodeLab::Str2WStr(((_CABencodeString *)bencode._dictionaries["announce"])->_string));
	}
	if (bencode.Find("announce-list"))
	{
		_CABencodeList * pBencodeList = (_CABencodeList *)bencode._dictionaries["announce-list"];
		for (std::list<_CABencode *>::iterator i = pBencodeList->_bencodeList.begin(); i != pBencodeList->_bencodeList.end(); i++)
		{
			_CABencodeList * pBencodeSubList = (_CABencodeList *)*i;
			for (std::list<_CABencode *>::iterator j = pBencodeSubList->_bencodeList.begin(); j != pBencodeSubList->_bencodeList.end(); j++)
			{
				_announceList.push_back(_CACodeLab::Str2WStr(((_CABencodeString *)*j)->_string));
			}
		}
	}
	return true;
}

bool _CATorrent::GetEncoding(_CABencodeDictionaries & bencode)
{
	if (bencode.Find("encoding"))
	{
		_encoding = _CACodeLab::Str2WStr(((_CABencodeString *)bencode._dictionaries["encoding"])->_string);
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
			_nameUTF8 = ((_CABencodeString *)pFileInfo->_dictionaries["name.utf-8"])->_string;
		else if (_encoding.compare(L"UTF-8") == 0)
		{
			_nameUTF8 = tstring;
			_name = _CACodeLab::UTF82WChar(tstring);
		}
		else
			_name = _CACodeLab::Str2WStr(tstring);
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
	//expectFile.path = _CACodeLab::Str2WStr(((_CABencodeString *)bencode._dictionaries["name"])->_string);
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
	if (_isMultiFiles)
	{
		//MultiFiles
		for (auto i : _expectFileList)
		{
			//_CAFile file(i.path);
		}
	}
	else
	{
		//SingleFile
		std::wstring fullPath(filePath);
		if (fullPath[filePath.length] != L'\\')
		{
			fullPath.append(L"\\");
		}
		//_CAFile file(fullPath.append(_name));
		fileStream.AddFile(fullPath.append(_name));
	}
	
	
	return false;
}

std::wstring _CATorrent::GetPath(_CABencodeList & bencode)
{
	std::wstring path;
	for (std::list<_CABencode *>::iterator i = bencode._bencodeList.begin(); i != bencode._bencodeList.end(); i++)
		if ((*i)->GetType() == _CABencode::BencodeType::BenString)
			path.append(L"\\").append(_CACodeLab::Str2WStr(((_CABencodeString *)(*i))->_string));
	return path;
}
