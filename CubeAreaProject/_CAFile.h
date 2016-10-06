#pragma once
#include "stdafx.h"
class _CAFile : 
	public _CAFileBase
{
public:
	_CAFile();
	_CAFile(std::wstring filePath);
	~_CAFile();

	std::wstring Suffix() { return _suffix; }
	__int64 Size() { return _size; }
	__int64 Offset() { return _offset; }
	virtual bool IsFolder();
	bool Eof();
	size_t Read(char * buffer, const size_t & offsetBegin, const size_t & expectedReadLength);
	// beta
	bool md5();
	bool sha1();
private:
	std::wstring _suffix;
	size_t _size;
	size_t _offset;
};

