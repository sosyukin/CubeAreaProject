// BTParser.h: interface for the CBTParser class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#if !defined(AFX_BTPARSER_H__D05551B8_1985_4B0C_BCD7_1C499FAB8ECD__INCLUDED_)
#define AFX_BTPARSER_H__D05551B8_1985_4B0C_BCD7_1C499FAB8ECD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdlib.h>
#include <stdio.h>
const int MAX_TORRENT_SIZE = 1024*1024*1024;
const int MAX_KEYWORD_SIZE = 256;
const int MAX_INT_SIZE = 10;
enum
{
    SUCCESS = 0,
    ERR_OPEN_FILE = -100,
    ERR_FILE_SIZE,
    ERR_READ_FILE,
    ERR_CODE,
};

class CBTParser  
{
public:
	CBTParser();
	~CBTParser();
public:
    int parse(char* sTorrentFile, char* sOutputFile);
private:
    int parseTorrent();
    int parseBencode();
    int parseInt();
    int parseStr(char* strOut = NULL);
    int parseList();
    int parseDict();
private:
    int outputBegin(char* str);
    int output(char* str);
    int output(char* str, int strLen);
    int outputEnd(char* str);
private:
    char* m_sTorrent;
    int   m_nIndex;
    int   m_nSize;
    FILE* m_pFile;
};

#endif // !defined(AFX_BTPARSER_H__D05551B8_1985_4B0C_BCD7_1C499FAB8ECD__INCLUDED_)
