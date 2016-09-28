#pragma once
#include "stdafx.h"
typedef unsigned char BYTE;
typedef long long INT64;
#define SHA1_LEN 20
typedef struct
{
	char * filePath;
	INT64 numBytes;
	int errorsFound;
} fileRecord;
class _CABTChecker
{
public:
	_CABTChecker();
	~_CABTChecker();
	// Step over an object (including all its embedded objects)
	int beStepOver(BYTE * benstr, int benstrLen, int benstrOffset);
	// Obtain the length of the string and a pointer to its beginning
	int beParseString(BYTE * benstr, int benstrLen, int benstrOffset, BYTE ** stringBegin, int * stringLen);
	// Return offset of an element in a dict, or -1 if not found
	int beFindInDict(BYTE * benstr, int benstrLen, int benstrOffset, BYTE * dictKey);
	// Extracts the integer
	int beParseInteger(BYTE * benstr, int benstrLen, int benstrOffset, INT64 * longOut);

	char* print64(INT64 val, char* buf, char useCommaDot);
	void backspaceProgressLine(int *showProgressChars);
private:
	char * torrentFile;
	char * contentPath;
	int contentPathLen;
	FILE * fp;
	BYTE * torrent;
	int torrentLen;
	int i;
	INT64 bytesRead;
	unsigned char sha1hash[SHA1_LEN];
	int torrentInfo;
	int ofs;
	int ofs2;
	int torrentPieceLen;
	int lastFile;
	int torrentFiles;
	int thisFileOffset;
	int multiFileTorrent;
	int filterMode;
	char * filterHash;
	int readLen;
	INT64 torrentPrivate;
	INT64 fileBytesExpected;
	INT64 fileBytesActual;
	INT64 fileBytesRead;
	BYTE * pieceList;
	int pieceListLen;
	BYTE * pieceBuf;
	int bytesInBuf;
	int bytesToRead;
	BYTE * rootName;
	int rootNameLen;
	BYTE * announce;
	int announceLen;
	int numPieces;
	int numFiles;
	INT64 pieceLen;
	BYTE * fileName;
	int fileNameLen;
	char * filePath;
	char * filePath2;
	char * filePathActual;
	int filePathMax;
	int filePathOfs;
	INT64 totalBytes;
	INT64 totalBytesDone;
	int piecesDone;
	int errorsFound;
	int errorsFoundThisFile;
	int piecesChecked;
	int firstFileThisPiece;
	int currentFile;
	fileRecord * fileRecordList;
	int maxFileRecords;
	int showProgressCount;
	int showPieceDetail;
	int useCommaDot;
	int thisPieceBad;
	int showProgressChars;
	char progressBuf[48];
	char p64Buf1[32];
	char p64Buf2[32];
};

