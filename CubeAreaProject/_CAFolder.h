#pragma once
#include "_CAFileBase.h"
class _CAFolder :
	public _CAFileBase
{
public:
	_CAFolder();
	_CAFolder(const std::wstring & filePath);
	~_CAFolder();
	std::vector<_CAFileBase *> _files;
private:
	// Files in folder.

public:
	// Return true if file is folder.
	virtual bool IsFolder();
	// Scan files in folder
	void Scan();
};

