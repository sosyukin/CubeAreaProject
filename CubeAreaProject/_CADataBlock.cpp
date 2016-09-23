#include "_CADataBlock.h"



_CADataBlock::_CADataBlock()
	: _blockSize(0)
{
}


_CADataBlock::~_CADataBlock()
{
	Clean();
}


// Get data.
bool _CADataBlock::GetData(const char *  stringData, long long DataSize)
{
	if (!DataSize)
		return true;
	_pblock = new char[DataSize];
	if (!_pblock)
		return false;
	_blockSize = DataSize;
	char * pDst = _pblock;
	const char * pSrc = stringData;
	for (long long i = 0; i < DataSize; i++)
		*pDst++ = *pSrc++;
	return true;
}


// Clean data.
void _CADataBlock::Clean()
{
	if (_blockSize != 0)
		delete[] _pblock;
}
