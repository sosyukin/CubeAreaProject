#include "stdafx.h"
#include "_CABTParser.h"
#define SHA1_LEN 20
#define DIR_SEPARATOR '\\'
_CABTParser::_CABTParser()
{
}


_CABTParser::~_CABTParser()
{
}


// Load a bt file.
bool _CABTParser::Open(const std::wstring & BTfilename)
{
	return _CACodeLab::ReadFileWithMemMapping(_BTstream, BTfilename);
}


// Check the BT file
bool _CABTParser::Check()
{
	BYTE * torrent = new BYTE[_BTstream.size()];
	BYTE * pbenStr = torrent;
	for (std::vector<BYTE>::iterator i = _BTstream.begin(); i != _BTstream.end(); i++)
	{
		*pbenStr++ = *i;
	}
	int torrentLen = (int)_BTstream.size();
	int i = beStepOver(torrent, torrentLen, 0);
	
	if (i != torrentLen)
	{
		std::cout << "error" << std::endl;
		return false;
	}
	int torrentInfo = beFindInDict(torrent, torrentLen, 0, (BYTE *)"info");
	if (torrentInfo < 0) {
		std::cout << "found info error" << std::endl;
		return false;
	}
	BYTE * rootName = NULL;
	int rootNameLen;
	int ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"name");
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &rootName, &rootNameLen);
	}
	if (ofs < 0) {
		std::cout << "found name error" << std::endl;
		return false;
	}
	INT64 torrentPrivate;
	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"private");
	if (ofs >= 0) {
		ofs = beParseInteger(torrent, torrentLen, ofs, &torrentPrivate);
	}
	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"length");
	INT64 totalBytes;
	INT64 fileBytesExpected;
	if (ofs >= 0) { // single file
		ofs = beParseInteger(torrent, torrentLen, ofs, &fileBytesExpected);
		totalBytes = fileBytesExpected;
	}
	else
	{
		//multi file.
		delete[] torrent;
		return false;
	}
	int filePathMax = 8192;
	char * filePath2 = new char[filePathMax];
	int torrentFiles = -1;
	if ((ofs < 0) && (torrentFiles < 0)) {
		printf("Unable to read \"info->length\" or \"info->files\" from torrent\n");
		return false;
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"pieces");
	BYTE * pieceList;
	int pieceListLen;
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &pieceList, &pieceListLen);
	}
	if (ofs < 0) {
		printf("Unable to read \"pieces\" from torrent\n");
		return false;
	}

	int numPieces = pieceListLen / SHA1_LEN;
	if (numPieces * SHA1_LEN != pieceListLen) {
		printf("Pieces list length is not a multiple of 20\n");
		return false;
	}
	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"piece length");
	INT64 pieceLen;
	if (ofs >= 0) {
		ofs = beParseInteger(torrent, torrentLen, ofs, &pieceLen);
	}
	if (ofs < 0) {
		printf("Unable to read \"piece length\" from torrent\n");
		return false;
	}
	ofs = beFindInDict(torrent, torrentLen, 0, (BYTE *)"announce");
	BYTE * announce;
	int announceLen;
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &announce, &announceLen);
	}
	if (ofs >= 0) {
		printf("Announce URL  : ");
		//fwrite(announce, announceLen, 1, stdout);
		printf("\n");
	}
	char * contentPath = "d:\\BT";
	char * filePath;
	int filePathOfs;
	filePath = new char[filePathMax];
	if (contentPath != NULL) 
	{ // single file torrent
		strcpy_s(filePath, filePathMax, contentPath);
		filePathOfs = (int)strlen(filePath);
		if ((filePathOfs > 0) && (filePath[filePathOfs - 1] != DIR_SEPARATOR)) {
			filePath[filePathOfs] = DIR_SEPARATOR;
			filePathOfs++;
		}
		memcpy_s(filePath + filePathOfs,filePathMax, rootName, rootNameLen);
		filePath[filePathOfs + rootNameLen] = '\0';
	}
	std::vector<BYTE> filestream;
	std::wstring wfilepath = _CACodeLab::Ansi2WChar(filePath, (int)strlen(filePath));
	_CACodeLab::ReadFileWithMemMapping(filestream, wfilepath);
	if (filestream.empty()) {
		_CACodeLab::ReadFileWithMemMapping(filestream, _CACodeLab::Ansi2WChar(contentPath, (int)strlen(contentPath)));
		if (!filestream.empty()) strcpy_s(filePath,filePathMax, contentPath);
	}
	if (filestream.empty()) {
		printf("Unable to open file %s or %s\n", contentPath, filePath);
		return false;
	}
	delete[] filePath;
	delete[] filePath2;
	delete[] torrent;
	return true;
}


int _CABTParser::beParseString(BYTE* benstr, int benstrLen, int benstrOffset, BYTE** stringBegin, int* stringLen) {
	int i;
	BYTE b;
	//BYTE* foundString;
	int foundLen = 0;
	for (i = benstrOffset;i<benstrLen;i++) {
		b = benstr[i];
		switch (b) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			foundLen = foundLen * 10 + (b - '0');
			break;
		case ':':
			if ((i + 1 + foundLen) > benstrLen) {
				return (-1);
			}
			else {
				*stringBegin = benstr + i + 1;
				*stringLen = foundLen;
				return (i + 1 + foundLen);
			}
		default:
			return (-1);
		}
	}
	return (-1);
}


int _CABTParser::beStepOver(BYTE* benstr, int benstrLen, int benstrOffset) {
	BYTE* bp;
	int ip;
	if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return (-1);
	switch (benstr[benstrOffset]) {
	case 'i':
		benstrOffset++;
		while (benstrOffset < benstrLen) {
			switch (benstr[benstrOffset]) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '-':
				benstrOffset++;
				break;
			case 'e':
				benstrOffset++;
				if (benstrOffset < benstrLen) return benstrOffset;
				else return (-1);
			default:
				return (-1);
			}
		}
		return (-1);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		benstrOffset = beParseString(benstr, benstrLen, benstrOffset, &bp, &ip);
		if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return(-1);
		else return benstrOffset;
	case 'l':
	case 'd':
		benstrOffset++;
		while (benstrOffset < benstrLen) {
			if (benstr[benstrOffset] == 'e') {
				benstrOffset++;
				if (benstrOffset <= benstrLen) return benstrOffset;
				else return (-1);
			}
			else {
				benstrOffset = beStepOver(benstr, benstrLen, benstrOffset);
				if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return (-1);
			}
		}
		return (-1);
	default:
		return (-1);
	}
}


int _CABTParser::beFindInDict(BYTE* benstr, int benstrLen, int benstrOffset, BYTE* dictKey) {
	BYTE* stringPtr;
	int stringLen;
	int dictKeyLen;

	if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return (-1);
	if (benstr[benstrOffset] != 'd') return (-1);
	dictKeyLen = (int)strlen((char *)dictKey);
	benstrOffset++;
	while ((benstrOffset >= 0) && (benstrOffset < benstrLen)) {
		if (benstr[benstrOffset] == 'e') return (-1);
		benstrOffset = beParseString(benstr, benstrLen, benstrOffset, &stringPtr, &stringLen);
		if (benstrOffset < 0) return (-1);
		if ((stringLen == dictKeyLen) && (memcmp(stringPtr, dictKey, stringLen) == 0)) {
			return (benstrOffset);
		}
		else {
			benstrOffset = beStepOver(benstr, benstrLen, benstrOffset);
		}
	}
	return (-1);
}


int _CABTParser::beParseInteger(BYTE* benstr, int benstrLen, int benstrOffset, INT64 * longOut) {
	INT64 foundValue = 0;
	INT64 negPos = 1;
	int i;
	BYTE b;
	for (i = benstrOffset;i<benstrLen;i++) {
		b = benstr[i];
		switch (b) {
		case 'i':
			if (i != benstrOffset) return (-1);
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			foundValue = foundValue * 10 + (b - '0');
			break;
		case '-':
			negPos = -1;
			break;
		case 'e':
			*longOut = foundValue * negPos;
			return (i + 1);
		default:
			return (-1);
		}
	}
	return (-1);
}