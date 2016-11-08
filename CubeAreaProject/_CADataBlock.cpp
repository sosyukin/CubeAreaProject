#include "stdafx.h"
#include "_CADataBlock.h"



_CADataBlock::_CADataBlock(const long long & blockSize)
	: _blockSize(0)
	, _pBlock(NULL)
	, _current(NULL)
	, _offset(0)
	, _dataSize(0)
{
	CreateBlock(blockSize);
}


_CADataBlock::~_CADataBlock()
{
	RemoveBlock();
}


// Get data.
/// TODO Remove this function.
bool _CADataBlock::GetData(const char *  stringData, long long DataSize)
{
	if (_blockSize < DataSize && DataSize <= DATABLOCK_MAX)
	{
		RemoveBlock();
		CreateBlock(DataSize);
	}
	if (!DataSize)
		return true;
	char * pDst = _pBlock;
	const char * pSrc = stringData;
	long long realSize = (DataSize < _blockSize) ? DataSize : _blockSize;
	for (long long i = 0; i < realSize; i++)
		*pDst++ = *pSrc++;
	return true;
}


bool _CADataBlock::CurrentByteIsNumber() const
{
	return (*_current >= '0') && (*_current <= '9');
}


_CADataBlock & _CADataBlock::operator++(int)
{
	if (_offset + 1 > _CADataBlock::_blockSize)
		throw std::exception("FileStream++ overflow.");
	_current++;
	_offset++;
	return *this;
}


_CADataBlock & _CADataBlock::operator+=(long long offset)
{
	if (_offset + offset > _CADataBlock::_blockSize)
		throw std::exception("FileStream++ overflow.");
	_current += offset;
	_offset += offset;
	return *this;
}


// Clean data.
void _CADataBlock::RemoveBlock()
{
	if (_blockSize != 0)
		delete[] _pBlock;
	_blockSize = 0;
	_pBlock = NULL;
	_current = NULL;
	_offset = 0;
	_dataSize = 0;
}


// Create a data block
long long _CADataBlock::CreateBlock(const long long & expectSize)
{
	if (_blockSize)
		RemoveBlock();
	long long realSize = (expectSize < DATABLOCK_MAX) ? expectSize : DATABLOCK_MAX;
	while (realSize > 0)
	{
		// TODO : Don't know how to
		try
		{
			_pBlock = new char[realSize];
			break;
		}
		catch (std::bad_alloc)
		{
			realSize /= 2;
		}
	}
	_blockSize = realSize;
	_current = _pBlock;
	return realSize;
}
