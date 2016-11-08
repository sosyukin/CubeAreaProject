#pragma once
#include "stdafx.h"

class _CAFileStream :
	public _CADataBlock
{
public:
	_CAFileStream(const long long & blockSize);
	_CAFileStream(const std::wstring & filePath);
	~_CAFileStream();
	// Current Pointer offset.
	std::list<_CAFile> _fileList;
	std::list<_CAFile>::iterator _currentFile;
	long long _pageNum;
	long long _dataLength;
	long long _currentPage;
	__int64 _endOffset;
	// Current Pointer ++
	_CAFileStream & operator++(int);
	_CAFileStream & operator+=(long long Offset);
	void AddFile(const std::wstring & filePath);
	void Append();
	void Update();
	std::string Fetch(__int64 ExpectedLength);
	bool GetData(const char * stringData, long long DataSize);
};

