#pragma once
#include <iostream>
#include "_CADataBlock.h"
class _CAFileStream :
	public _CADataBlock
{
public:
	_CAFileStream();
	~_CAFileStream();
	// Current Pointer.
	char *_Current;
	// Current Pointer offset.
	long long _Offset;
	// Current Pointer ++
	_CAFileStream & operator++(int);
	bool GetData(const char * stringData, long long DataSize);
	_CAFileStream & operator+=(long long Offset);
	bool CurrentByteIsNumber() const;
};

