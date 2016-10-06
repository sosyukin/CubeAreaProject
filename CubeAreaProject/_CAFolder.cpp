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
		if (!CreateDirectory(filePath.c_str(), NULL))
		{
			throw std::exception("Can not create this folder.");
		}
	}
	Scan();
}


_CAFolder::~_CAFolder()
{
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
	LARGE_INTEGER filesize;
	_files.clear();
	HANDLE hFind = FindFirstFile(std::wstring(_path).append(L"\\*").c_str(), &ffd);
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				_CACodeLab::FileOut(std::wstring(L"(folder)\t").append(ffd.cFileName).append(L"\n"), L"D:\\FolderScan.txt");
				_files.push_back(_CAFileBase(_CAFolder(std::wstring(_path).append(L"\\").append(ffd.cFileName))));
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_CACodeLab::FileOut(std::wstring(L"(file)\t\t").append(ffd.cFileName).append(L"\n"), L"D:\\FolderScan.txt");
			//_CAFile file(std::wstring(_path).append(L"\\").append(ffd.cFileName));
			//_files.push_back(_CAFileBase(file));
			_files.push_back(_CAFileBase(_CAFile(std::wstring(_path).append(L"\\").append(ffd.cFileName))));
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
}
