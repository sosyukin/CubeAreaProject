#pragma once
#include "stdafx.h"
class _CAFileBase
{
public:

	_CAFileBase();
	_CAFileBase(const std::wstring & filePath);
	~_CAFileBase();
	// For debug
	// Get file name.
	virtual std::wstring Name() { return _name; };
	// Get file path
	virtual std::wstring Path() { return _path; };
	// Check file is folder.
	virtual bool IsFolder();
	virtual bool Rename(const std::wstring & newName) = 0;
protected:
	// File name.
	std::wstring _name;
	// File path.
	std::wstring _path;
	std::wstring _parentFolder;
private:
	// Check file exist.
	virtual bool IsExist();
public:
	// Open a file
	virtual bool Open(const std::wstring & filePath);
};

