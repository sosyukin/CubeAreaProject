#include "stdafx.h"
#include "_CAFolder.h"
_CAFolder::_CAFolder()
	: _CAFileBase()
{
}

_CAFolder::_CAFolder(const std::wstring & filePath)
	: _CAFileBase(filePath)
{
	if (!_CAFileBase::IsFolder())
	{
		throw std::exception("Folder not exist.");
		/*if (!CreateDirectory(filePath.c_str(), NULL))
		{
			throw std::exception("Can not create this folder.");
		}*/
	}
	Scan();
}


_CAFolder::~_CAFolder()
{
	ClearFileList();
}


// Return true if file is folder.
bool _CAFolder::IsFolder()
{
	return true;
}


// Scan files in folder
void _CAFolder::Scan()
{
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	//_files.clear();
	ClearFileList();
	HANDLE hFind = FindFirstFile(std::wstring(_path).append(L"\\*").c_str(), &ffd);
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				_files.push_back(new _CAFolder(std::wstring(_path).append(L"\\").append(ffd.cFileName)));
			}
		}
		else
		{
			_files.push_back(new _CAFile(std::wstring(_path).append(L"\\").append(ffd.cFileName)));
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
}

bool _CAFolder::Find(const std::wstring & fileName, std::vector<_CAFile *> & fileList)
{
	return false;
}

bool _CAFolder::GetFileList(std::vector<_CAFile *> & fileList)
{
	for (auto i : _files)
	{
		_CAFolder * pfolder = dynamic_cast<_CAFolder *>(i);
		if (pfolder)
		{
			pfolder->GetFileList(fileList);
			continue;
		}
		_CAFile * pfile = dynamic_cast<_CAFile *>(i);
		if (pfile)
		{
			fileList.push_back(pfile);
			continue;
		}
		_CALog::Log("Unable to get file\n");
		return false;
	}
	return true;
}

bool _CAFolder::GetFolderList(std::vector<_CAFolder*>& folderList)
{
	for (auto i : _files)
	{
		_CAFolder * pfolder = dynamic_cast<_CAFolder *>(i);
		if (pfolder)
		{
			folderList.push_back(pfolder);
		}
	}
	return true;
}

bool _CAFolder::GetFileList(const std::wstring & constraintStr, std::vector<_CAFile*>& fileList, bool (*ConstraintFunction)(const std::wstring &,const std::wstring &), _CAFile::ATTR attr)
{
	for (auto i : _files)
	{
		_CAFolder * pfolder = dynamic_cast<_CAFolder *>(i);
		if (pfolder)
		{
			pfolder->GetFileList(constraintStr, fileList, ConstraintFunction, attr);
			continue;
		}
		_CAFile * pfile = dynamic_cast<_CAFile *>(i);
		if (pfile)
		{
			if (pfile->Constraint(constraintStr, ConstraintFunction, attr))
			{
				fileList.push_back(pfile);
			}
			continue;
		}
		_CALog::Log("Unable to get file\n");
		return false;
	}
	return true;
}

bool _CAFolder::Rename(const std::wstring & newName)
{
	return false;
}


// Open a folder
bool _CAFolder::Open(const std::wstring & filePath)
{
	if (_CAFileBase::Open(filePath) && _CAFileBase::IsFolder())
	{
		return true;
	}
	return false;
}


// release file list
bool _CAFolder::ClearFileList()
{
	for (auto i : this->_files)
		delete i;
	_files.clear();
	return true;
}
