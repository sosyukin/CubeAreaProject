#pragma once
#include "stdafx.h"
class _CADataBlock
{
	
public:
	_CADataBlock(const long long & blockSize);
	~_CADataBlock();
	// Create a data block
	long long CreateBlock(const long long & expectSize);
	// Clean data.
	void RemoveBlock();
	// Get data.
	bool GetData(const char * stringData, long long DataSize);
	bool CurrentByteIsNumber() const;
	virtual _CADataBlock & operator++(int);
	virtual _CADataBlock & operator+=(long long offset);
	
	// Data block begin point.
	char * _pBlock;
	char * _current;
	// Data block size.
	long long _blockSize;
	long long _offset;
	long long _dataSizeInBlock;

};

