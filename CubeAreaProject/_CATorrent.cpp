#include "stdafx.h"
#include "_CATorrent.h"



_CATorrent::_CATorrent(const std::wstring & torrentPath)
	: _totalLength(0)
	, _encoding(L"UTF-8")
	, _isMultiFiles(false)
	, _torrentFile(torrentPath)
	, _havePaddingFiles(false)
	, _haveError(false)
{
	_content.Parse(_CAFileStream(torrentPath));
	//_content.Output(std::wstring(L"D:\\").append(_torrentFile.Name()).append(L".txt"), 0);
	GetAnnounce(_content);
	GetEncoding(_content);
	if (!GetFileInfo(_content))
	{
		_haveError = true;
	};
}

_CATorrent::_CATorrent(_CABencodeDictionary & bencode)
	: _totalLength(0)
	, _encoding(L"UTF-8")
	, _isMultiFiles(false)
	, _havePaddingFiles(false)
	, _haveError(false)
{
	GetAnnounce(bencode);
	GetEncoding(bencode);
	if (!GetFileInfo(_content))
	{
		_haveError = true;
	};
}

_CATorrent::~_CATorrent()
{
}


bool _CATorrent::GetAnnounce(_CABencodeDictionary & bencode)
{
	if (bencode.Find("announce"))
	{
		_announceList.push_back(_CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionary["announce"])->_string));
	}
	if (bencode.Find("announce-list"))
	{
		_CABencodeList * pBencodeList = (_CABencodeList *)bencode._dictionary["announce-list"];
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

bool _CATorrent::GetEncoding(_CABencodeDictionary & bencode)
{
	if (bencode.Find("encoding"))
	{
		_encoding = _CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionary["encoding"])->_string);
		return true;
	}
	else //_CALog::Log(std::wstring(_torrentFile.Name()).append(L" no encoding\n"), L"d:\\BTTest\\Log\\EncodingCheck.txt");
		return false;
}

bool _CATorrent::GetFileInfo(_CABencodeDictionary & bencode)
{
	if (bencode.Find("info"))
	{
		_CABencodeDictionary * pFileInfo = (_CABencodeDictionary *)bencode._dictionary["info"];
		if (pFileInfo->Find("files"))
		{
			//multi file.
			if (((_CABencodeDictionary *)pFileInfo->_dictionary["files"])->GetType() == _CABencode::BencodeType::BenList)
				GetMutliFilesInfo(*(_CABencodeList *)pFileInfo->_dictionary["files"]);
		}
		else
		{
			//single file.
			GetSingleFileInfo(*pFileInfo);
		}
		if (!pFileInfo->Find("name"))
			return false;
		std::string tstring = ((_CABencodeString *)pFileInfo->_dictionary["name"])->_string;
		if (pFileInfo->Find("name.utf-8"))
		{
			//_name = _CACharConversion::ansi2unicode(tstring);
			_nameUTF8 = ((_CABencodeString *)pFileInfo->_dictionary["name.utf-8"])->_string;
			_name = _CACharConversion::utf82unicode(_nameUTF8);
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
		_pieceLength = ((_CABencodeInteger *)pFileInfo->_dictionary["piece length"])->_value;
		if (pFileInfo->Find("pieces"))
			if (pFileInfo->_dictionary["pieces"]->GetType() == _CABencode::BencodeType::BenString)
				return GetPieces(*(_CABencodeString *)pFileInfo->_dictionary["pieces"]);
	}
	if (_havePaddingFiles)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

bool _CATorrent::GetSingleFileInfo(_CABencodeDictionary & bencode)
{
	//ExpectFile expectFile;
	if (!bencode.Find("length"))
		return false;
	//expectFile.length = ((_CABencodeInteger *)bencode._dictionary["length"])->_value;
	_totalLength += ((_CABencodeInteger *)bencode._dictionary["length"])->_value;

	//if (!bencode.Find("name"))
	//	return false;
	//expectFile.path = _CACharConversion::ansi2unicode(((_CABencodeString *)bencode._dictionary["name"])->_string);
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
		if ((*i)->GetType() == _CABencode::BencodeType::BenDictionary)
		{
			_CABencodeDictionary * pFileBencode = (_CABencodeDictionary *)(*i);
			if (!pFileBencode->Find("length"))
				return false;
			expectFile.length = ((_CABencodeInteger *)pFileBencode->_dictionary["length"])->_value;
			_totalLength += expectFile.length;
			if (!pFileBencode->Find("path"))
				return false;
			expectFile.path = GetPath(*(_CABencodeList *)pFileBencode->_dictionary["path"]);
			if (pFileBencode->Find("filehash"))
			{
				expectFile.fileHash = ((_CABencodeString *)pFileBencode->_dictionary["filehash"])->_string;
			}
		}
		if (1 == expectFile.path.find_first_of(L"_____padding_file_", 0))
		{
			_havePaddingFiles = true;
			return false;
		}
		else
		{
			_expectFileList.push_back(expectFile);
		}
	}
	return true;
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
	bool checkState = true;
	_CAFileStream fileStream(_totalLength);
	_filePath = filePath;
	if (_filePath[filePath.length() - 1] != L'\\')
	{
		_filePath.append(L"\\");
	}
	if (_isMultiFiles)
	{
		//MultiFiles
		// Check download folder is exist.
		_filePath.append(_name);
		_CAFolder folderCheck;
		try
		{
			folderCheck.Open(_filePath);
		}
		catch (const std::exception&)
		{
			throw std::exception("File not exist.");
		}
		//Prepare checking
		if (_havePaddingFiles)
		{
			// TODO : checking for padding files
			for (auto itExpectFile = _expectFileList.begin(); itExpectFile != _expectFileList.end(); itExpectFile++)
			{
				itExpectFile->path = std::wstring(_filePath).append(itExpectFile->path);
			}
		}
		else
		{
			// Add files to filestream and check files exist.
			try
			{
				for (auto i : _expectFileList)
				{
					fileStream.AddFile(std::wstring(_filePath).append(i.path));
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
				throw std::exception("File missing.");
			}
		}
	}
	else
	{
		//SingleFile
		_CAFile fileCheck;
		fileCheck.Open(_filePath.append(_name));
		fileStream.AddFile(_filePath);
	}

	if (_havePaddingFiles)
	{
		CSHA1 sha1;
		unsigned char shastr[21];
		shastr[20] = '\0';
		for (auto expectFile : _expectFileList)
		{
			_CAFileStream fileStream(expectFile.path);
			std::string binary = fileStream.Fetch(expectFile.length);
			sha1.Reset();
			sha1.Update((unsigned char *)binary.c_str(), (unsigned int)expectFile.length);
			sha1.Final();
			sha1.GetHash(shastr);
			TCHAR report[40];
			sha1.ReportHash(report);
			if (strncmp(expectFile.fileHash.c_str(), (char *)shastr, 20) != 0)
			{
				throw std::exception(_CACharConversion::unicode2ansi(std::wstring(_name).append(L" piece error.")).c_str());
			}
			else
			{
				std::cout << _CACharConversion::unicode2ansi(expectFile.path).append(" checked OK.") << std::endl;
			}
		}
		return checkState;
	}
	else
	{
		// Output checking status
		std::cout << "Checking... " << _CACharConversion::unicode2ansi(_name) << std::endl;
		std::cout.width(3);
		std::cout << 0 << "%";
		CSHA1 sha1;
		size_t pieceNumber = 0;
		unsigned char shastr[21];
		shastr[20] = '\0';
		int percent;
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
				throw std::exception(_CACharConversion::unicode2ansi(std::wstring(_name).append(L" piece error.")).c_str());
			}
			else
			{
				percent = pieceNumber * 100 / _pieceList.size();
				std::cout << "\b\b\b\b";
				std::cout.width(3);
				std::cout << percent << "%";
			}
		}
		std::cout << std::endl;
		return checkState;
	}
}

bool _CATorrent::Rename()
{
	return _torrentFile.Rename(std::wstring(_name).append(L".").append(_torrentFile.Suffix()));
}

bool _CATorrent::NoError()
{
	return !_haveError;
}

bool _CATorrent::HavePaddingFiles()
{
	return _havePaddingFiles;
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

std::wstring _CATorrent::GetDownloadPath()
{
	return _filePath;
}
