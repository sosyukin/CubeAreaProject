#pragma once
#include <vector>
#include "_CAFileBase.h"
#include "_CAFile.h"
class _CAFolder :
	public _CAFileBase
{
public:
	_CAFolder();
	_CAFolder(std::wstring filePath);
	~_CAFolder();
private:
	// Files in folder.
	std::vector<_CAFileBase> _files;
public:
	// Return true if file is folder.
	virtual bool IsFolder();
	// Scan files in folder
	void Scan();
};

