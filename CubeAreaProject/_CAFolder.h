#pragma once
#include "_CAFileBase.h"
#include "_CAFile.h"
class _CAFolder :
	public _CAFileBase
{
public:
	_CAFolder();
	_CAFolder(const std::wstring & filePath);
	~_CAFolder();
	std::vector<_CAFileBase *> _files;
	// Return true if file is folder.
	virtual bool IsFolder();
	// Scan files in folder
	void Scan();
	bool Find(const std::wstring & fileName, std::vector<_CAFile *> & fileList);
	bool GetFileList(std::vector<_CAFile *> & fileList);

	bool GetFileList(const std::wstring & constraintStr, std::vector<_CAFile*>& fileList, bool(*ConstraintFunction)(const std::wstring &, const std::wstring &), _CAFile::ATTR attr);
	virtual bool Rename(const std::wstring & newName);
};

