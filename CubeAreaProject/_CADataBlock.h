#pragma once
class _CADataBlock
{
	
public:
	_CADataBlock();
	~_CADataBlock();
	// Get data.
	bool GetData(const char * stringData, long long DataSize);
	// Clean data.
	void Clean();
protected:
	// Data block begin point.
	char *_pblock;
	// Data block size.
	long long _blockSize;
};

