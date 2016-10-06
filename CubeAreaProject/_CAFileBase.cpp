#include "stdafx.h"
#include "_CAFileBase.h"


_CAFileBase::_CAFileBase()
	: _name(std::wstring())
	, _path(std::wstring())
{
}

_CAFileBase::_CAFileBase(const std::wstring & filePath)
	: _path(filePath)
	, _name(filePath.substr(filePath.find_last_of('\\') + 1))
{
	IsExist();
}


_CAFileBase::~_CAFileBase()
{
}


bool _CAFileBase::IsFolder()
{
	/*DWORD hFile = NULL;
	struct _finddata_t fileinfo;
	if ((hFile = _findfirst(_CACodeLab::WChar2Ansi(_path.c_str()).c_str(), &fileinfo)) != -1)
	{
		if (fileinfo.attrib & _A_SUBDIR)
		{
			_findclose(hFile);
			return true;
		}
	}
	_findclose(hFile);*/
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(_path.c_str(), &ffd);
	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFind);
		return true;
	}
	FindClose(hFind);
	return false;
}


bool _CAFileBase::IsExist()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(_path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
		//throw std::exception(_CACodeLab::WChar2Ansi(_path.append(L" not found.").c_str()).c_str());
	}
	FindClose(hFind);
	return true;
}

