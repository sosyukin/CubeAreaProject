#pragma once
#include <stdio.h>
#include <string>
#include <fstream>
#include "_CAFileBase.h"
#include "_CACodeLab.h"

class _CAFile : 
	public _CAFileBase
{
public:
	_CAFile();
	_CAFile(std::wstring filePath);
	~_CAFile();

	// Get suffix
	std::wstring Suffix() { return _suffix; }
	// Get file size.
	__int64 Size() { return _size; }
	// Return true if file is folder.
	virtual bool IsFolder();
private:
	// Suffix name
	std::wstring _suffix;
	// File size
	__int64 _size;
public:
	// log md5
	bool md5();

	// log sha1
	bool sha1();
};

