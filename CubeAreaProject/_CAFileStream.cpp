#include "stdafx.h"
#include "_CAFileStream.h"



_CAFileStream::_CAFileStream(const long long & blockSize)
	: _CADataBlock(blockSize)
	, _currentFile(_fileList.end())
{
}


_CAFileStream::~_CAFileStream()
{
}


// Current Pointer ++
_CAFileStream & _CAFileStream::operator++(int)
{
	return *this;
}


bool _CAFileStream::GetData(const char * stringData, long long DataSize)
{
	if (!_CADataBlock::GetData(stringData, DataSize))
		return false;
	_current = _pBlock;
	_offset = 0;
	return true;
}


_CAFileStream & _CAFileStream::operator+=(long long Offset)
{
	return *this;
}

void _CAFileStream::AddFile(const std::wstring & filePath)
{
	_CAFile file(filePath);
	_dataLength += file.Size();
	_pageNum = ceil((double)_dataLength / _blockSize);
	_fileList.push_back(file);
	if (_currentFile == _fileList.end())
	{
		_currentFile--;
	}
	Append();
}

void _CAFileStream::Append()
{
	while (_dataSizeInBlock < _blockSize && _currentFile != _fileList.end())
	{
		__int64 offset = _currentFile->Read(_current, _currentFileOffset, _blockSize - _dataSizeInBlock);
		this->operator+=(offset);
		_dataSizeInBlock += offset;
		if (_currentFileOffset + offset < _currentFile->Size())
		{
			break;
		}
		_currentFile++;
	}
}
