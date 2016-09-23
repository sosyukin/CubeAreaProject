#include "_CAFileStream.h"



_CAFileStream::_CAFileStream()
	: _Offset(0)
{
}


_CAFileStream::~_CAFileStream()
{
}


// Current Pointer ++
_CAFileStream & _CAFileStream::operator++(int)
{
	if (_Offset + 1 >= _CADataBlock::_blockSize)
		throw std::exception("FileStream++ overflow.");
	_Current++;
	_Offset++;
	return *this;
}


bool _CAFileStream::GetData(const char * stringData, long long DataSize)
{
	if (!_CADataBlock::GetData(stringData, DataSize))
		return false;
	_Current = _pblock;
	_Offset = 0;
	return true;
}


_CAFileStream & _CAFileStream::operator+=(long long Offset)
{
	if (_Offset + Offset >= _CADataBlock::_blockSize)
		throw std::exception("FileStream++ overflow.");
	_Current += Offset;
	_Offset += Offset;
	return *this;
}


bool _CAFileStream::CurrentByteIsNumber()
{
	return (*_Current >= '0') && (*_Current <='9');
}
