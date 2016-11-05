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
	Diff();
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

bool _CAFile::Diff()
{
	//size_t test1 = _name.find_last_of('\\');
	std::wstring folderPath = _path.substr(0, _path.find_last_of('\\') + 1);
	folderPath = folderPath.substr(folderPath.find_first_of('\\') + 1);
	folderPath = folderPath.substr(folderPath.find_first_of('\\') + 1);
	std::wstring TargetPath = std::wstring(L"d:\\DaCapo\\").append(folderPath);
	sha1();
	_CADB db;
	//_bstr_t	Find(L"Select * from FileSHA1 where SHA1='");
	//Find += _bstr_t(_sha1Report.c_str());
	//Find += _bstr_t(L"\'");
	_variant_t columnList[2] = { L"SHA1", L"Path" };
	_variant_t valueList[2] = { _sha1Report.c_str(), _path.c_str() };
	if (!db.Insert(L"FileSHA1", std::vector<_variant_t>(columnList, columnList + 2), std::vector<_variant_t>(valueList, valueList + 2)))
		_CACodeLab::FileOut(std::wstring(_path).append(L" ").append(_sha1Report.c_str()).append(L"\n"), L"D:\\sha1.txt");
	//_bstr_t FileNum(L"Select COUNT(*) from FileIndex");
	//if (db.EmptySet(Find))
	//{
	//	//_variant_t result = db.pRecordset->GetCollect(db.pRecordset->Fields->GetItem(_variant_t((long)0))->Name);
	//	//_bstr_t Insert(L"INSERT INTO fileIndex (Path,SHA1) VALUES('");
	//	//std::wstring pathSQL(_path);
	//	//_CACodeLab::EscapeSequence(pathSQL);
	//	//Insert += _bstr_t(pathSQL.c_str());
	//	//Insert += _bstr_t(L"','");
	//	//Insert += _bstr_t(_sha1Report.c_str());
	//	//Insert += _bstr_t(L"');");
	//	////db.Execute(Insert);
	//	_variant_t columnList[2] = { L"SHA1", L"Path" };
	//	_variant_t valueList[2] = { _sha1Report.c_str(), _path.c_str() };
	//	db.Insert(L"fileIndex", std::vector<_variant_t>(columnList, columnList + 2), std::vector<_variant_t>(valueList, valueList + 2));
	//}
	//else
	//{
	//	//_wsystem(std::wstring(L"C:\\FastCopy313_x64\\FastCopy.exe /cmd=diff /auto_close /log=FALSE /no_ui \"").append(this->_path).append(L"\" /to=\"").append(TargetPath).append(L"\"").c_str());
	//	_CACodeLab::FileOut(std::wstring(_path).append(L" ").append(_sha1Report.c_str()).append(L"\n"), L"D:\\sha1.txt");
	//	return false;
	//	
	//}
	return true;
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
	sha1.ReportHashStl(_sha1Report, CSHA1::REPORT_HEX_SHORT);
	//_CACodeLab::FileOut(std::wstring(_path).append(L" ").append(strReport.c_str()).append(L"\n"), L"D:\\sha1.txt");
	return false;
}
