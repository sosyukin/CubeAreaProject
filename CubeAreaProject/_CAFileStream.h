#pragma once
#include "stdafx.h"

class _CAFileStream :
	public _CADataBlock
{
public:
	_CAFileStream(const long long & blockSize);
	~_CAFileStream();
	// Current Pointer offset.
	std::list<_CAFile> _fileList;
	std::list<_CAFile>::iterator _currentFile;
	long long _pageNum;
	long long _dataLength;
	long long _currentPage;
	long long _currentFileOffset;
	// Current Pointer ++
	_CAFileStream & operator++(int);
	_CAFileStream & operator+=(long long Offset);
	void AddFile(const std::wstring & filePath);
	void Append();
	bool GetData(const char * stringData, long long DataSize);
	
};

