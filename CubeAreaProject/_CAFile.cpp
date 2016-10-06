#include "stdafx.h"
#include "_CAFile.h"

_CAFile::_CAFile()
	: _CAFileBase()
	, _suffix(std::wstring())
{
}


_CAFile::_CAFile(const std::wstring & filePath)
	: _CAFileBase(filePath)
	, _suffix(std::wstring())
	, _offset(0)
{
	if (_CAFileBase::IsFolder())
		throw std::exception("This is a folder.");
	_suffix = _name.substr(_name.find_last_of('.') + 1);
	//_size = _CACodeLab::CLGetFileSize(_path);
	_CACodeLab::GetFileLength(_size, _path);
}

_CAFile::~_CAFile()
{
}


// Return true if file is folder.
bool _CAFile::IsFolder()
{
	return false;
}

bool _CAFile::Eof()
{
	return _size == _offset;
}

size_t _CAFile::Read(char * buffer, const size_t & offsetBegin, const size_t & expectedReadLength)
{
	__int64 actualReadLength = min(_size - _offset, expectedReadLength);
	if (actualReadLength)
	{
		std::ifstream file(_path, std::ios::binary);
		file.seekg(offsetBegin);
		file.read(buffer, actualReadLength);
		file.close();
	}
	_offset += actualReadLength;
	return actualReadLength;
}


// log md5
bool _CAFile::md5()
{
	//_wsystem(std::wstring(L"md5sums -u \"").append(this->_path).append(L"\" >> d:\\md5log.txt").c_str());
	//_CACodeLab::FileOut(std::wstring(_path).append(L" ").append(_CAMD5(std::ifstream(_path, std::ios::binary)).toWString()).append(L"\n"), L"D:\\md5.txt");
	return true;
}


// // log sha1
bool _CAFile::sha1()
{
	CSHA1 sha1;
	sha1.HashFile(_path.c_str());
	sha1.Final();
	std::wstring strReport;
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	_CACodeLab::FileOut(std::wstring(_path).append(L" ").append(strReport.c_str()).append(L"\n"), L"D:\\sha1.txt");
	return false;
}
