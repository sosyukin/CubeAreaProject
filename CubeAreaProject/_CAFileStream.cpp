#include "stdafx.h"
#include "_CAFileStream.h"



_CAFileStream::_CAFileStream(const long long & blockSize)
	: _CADataBlock(blockSize)
	, _currentFile(_fileList.end())
	, _dataLength(0)
	, _currentPage(0)
	, _endOffset(0)
{
}


_CAFileStream::~_CAFileStream()
{
}


// Current Pointer ++
_CAFileStream & _CAFileStream::operator++(int)
{
	_CADataBlock::operator++(1);
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

std::string _CAFileStream::Fetch(__int64 ExpectedLength)
{
	std::string buffer;
	__int64 fetchedLength = 0;
	__int64 actualFetchLength = min(ExpectedLength, _dataLength - _currentPage * _blockSize - _offset);
	while (fetchedLength < actualFetchLength)
	{
		Update();
		__int64 readLength = min(actualFetchLength - fetchedLength, _dataSize - _offset);
		buffer.append(_current, readLength);
		(*this) += readLength;
		fetchedLength += readLength;
	}
	return buffer;
}


_CAFileStream & _CAFileStream::operator+=(long long Offset)
{
	_CADataBlock::operator+=(Offset);
	return *this;
}

void _CAFileStream::AddFile(const std::wstring & filePath)
{
	_CAFile file(filePath);
	_dataLength += file.Size();
	_pageNum = (__int64)ceil((double)_dataLength / _blockSize);
	_fileList.push_back(file);
	if (_currentFile == _fileList.end())
	{
		_currentFile--;
	}
	Append();
}

void _CAFileStream::Append()
{
	while (_dataSize < _blockSize && _currentFile != _fileList.end())
	{
		size_t readOffset = _currentFile->Read(_pBlock + _endOffset, _currentFile->Offset(), _blockSize - _dataSize);
		_dataSize += readOffset;
		_endOffset += readOffset;

		if (_currentFile->Eof())
		{
			_currentFile++;
		}
	}
}

void _CAFileStream::Update()
{
	if (_offset >= _blockSize)
	{
		_currentPage++;
		_offset = 0;
		_dataSize = 0;
		_endOffset = 0;
		_current = _pBlock;
		Append();
	}
}
