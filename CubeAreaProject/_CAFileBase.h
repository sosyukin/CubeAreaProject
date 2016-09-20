#pragma once
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <io.h>
#include "_CACodeLab.h"

class _CAFileBase
{
public:
	_CAFileBase();
	_CAFileBase(std::wstring filePath);
	~_CAFileBase();
	// For debug
	// Get file name.
	virtual std::wstring Name() { return _name; };
	// Get file path
	virtual std::wstring Path() { return _path; };
	// Check file is folder.
	virtual bool IsFolder();

protected:
	// File name.
	std::wstring _name;
	// File path.
	std::wstring _path;
private:
	// Check file exist.
	virtual bool IsExist();
};

