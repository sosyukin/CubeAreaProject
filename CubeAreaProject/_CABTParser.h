#pragma once
#include <vector>
class _CABTParser
{
private:
	typedef unsigned char BYTE;
	std::vector<BYTE> _BTstream;

public:
	_CABTParser();
	~_CABTParser();
	// Load a bt file.
	bool Open(const std::wstring & BTfilename);
	// Check the BT file
	bool Check();
	// Old parse string
	int beParseString(BYTE* benstr, int benstrLen, int benstrOffset, BYTE** stringBegin, int* stringLen);
	int beStepOver(BYTE* benstr, int benstrLen, int benstrOffset);
	int beFindInDict(BYTE* benstr, int benstrLen, int benstrOffset, BYTE* dictKey);
	int beParseInteger(BYTE * benstr, int benstrLen, int benstrOffset, INT64 * longOut);

};

