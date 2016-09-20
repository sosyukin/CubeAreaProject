// BTParser.cpp: implementation of the CBTParser class.
//
//////////////////////////////////////////////////////////////////////

#include "BTParser.h"

#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <share.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define isdigit(x) ((x) >= '0' && (x) <= '9')
CBTParser::CBTParser()
{
    m_sTorrent = new char[MAX_TORRENT_SIZE];
    m_nIndex   = 0;
    m_nSize    = 0;
    m_pFile    = NULL;
}

CBTParser::~CBTParser()
{
    delete[] m_sTorrent;
    m_sTorrent = NULL;
}

int CBTParser::parse(char* sTorrentFile, char* sOutputFile)
{
    //int file = _open(sTorrentFile, O_RDONLY | O_BINARY);
	int file;
	_sopen_s(&file, sTorrentFile, O_RDONLY | O_BINARY, _SH_DENYNO, _S_IREAD);
    if(-1 == file)
    {
        return ERR_OPEN_FILE;
    }
    m_nSize = _filelength(file);
    if(m_nSize >= MAX_TORRENT_SIZE || m_nSize <= 0)
    {
        _close(file);
        return ERR_FILE_SIZE;
    }
    int iRead = 0;
    int iRet = SUCCESS;
    while(iRead != m_nSize)
    {
        iRet = _read(file, m_sTorrent+iRead, m_nSize-iRead);
        if(-1 == iRet)
        {
            _close(file);
            return ERR_READ_FILE;
        }
        iRead += iRet;
    }

    m_sTorrent[m_nSize] = '\0';
    _close(file);
    m_nIndex = 0;
    fopen_s(&m_pFile, sOutputFile, "w");
    if(NULL == m_pFile)
    {
        return ERR_OPEN_FILE;
    }
    fprintf(m_pFile, "<?xml version=\"1.0\" encoding=\"gb2312\"?>");
    fprintf(m_pFile, "<torrent>");
    iRet = parseTorrent();
    fprintf(m_pFile, "</torrent>");
    
    fclose(m_pFile);
    m_pFile = NULL;
    return iRet;
}
int CBTParser::parseTorrent()
{
    int iRet = SUCCESS;

    while(m_nIndex < m_nSize)
    {
        iRet = parseBencode();
        if(iRet != SUCCESS)
        {
            break;
        }
    }

    return iRet;
}

//d<bencoding×Ö·û´®><bencoding±àÂëÀàÐÍ>e 
int CBTParser::parseDict()
{
    int iRet = SUCCESS;
    char sAttr[MAX_KEYWORD_SIZE] = {0};
    if('d' != m_sTorrent[m_nIndex++])
    {
        return ERR_CODE;
    }
    while('e' != m_sTorrent[m_nIndex])
    {
        if(SUCCESS != (iRet = parseStr(sAttr)))
        {
            break;
        }

        if(SUCCESS != (iRet = outputBegin(sAttr)))
        {
            break;
        }

        if(SUCCESS != (iRet = parseBencode()))
        {
            break;
        }

        if(SUCCESS != (iRet = outputEnd(sAttr)))
        {
            break;
        }
    }
    ++m_nIndex;
    return iRet;
}

int CBTParser::parseBencode()
{
    int iRet = SUCCESS;
    switch(m_sTorrent[m_nIndex])
    {
    case 'i':
        iRet = parseInt();
    	break;
    case 'l':
        iRet = parseList();
    	break;
    case 'd':
        iRet = parseDict();
        break;
    default:
        if(isdigit(m_sTorrent[m_nIndex]))
        {
            iRet = parseStr();
        }
        else
        {
            iRet = ERR_CODE;
        }
        break;
    }

    return iRet;
}

//i<ÕûÊý>e
int CBTParser::parseInt()
{
    if(m_sTorrent[m_nIndex] != 'i')
    {
        return ERR_CODE;
    }
    int nStartPos = ++m_nIndex;
    while(m_sTorrent[m_nIndex] != 'e')
    {
        if(!isdigit(m_sTorrent[m_nIndex]))
        {
            if(nStartPos == m_nIndex)
            {
                if('+' == m_sTorrent[m_nIndex]
                    ||'-' == m_sTorrent[m_nIndex])
                {
                    continue;
                }
            }
            return ERR_CODE;
        }
        ++m_nIndex;
    }
    int nEndPos = m_nIndex-1;
    ++m_nIndex;

    return output(m_sTorrent+nStartPos, nEndPos-nStartPos+1);
}

//<×Ö·û´®³¤¶È>£º<×Ö·û´®>
int CBTParser::parseStr(char* strOut)
{
    char tmp[MAX_INT_SIZE+1] = {0};
    int i = 0;
    while(m_sTorrent[m_nIndex] != ':')
    {
        if(!isdigit(m_sTorrent[m_nIndex]))
        {
            return ERR_CODE;
        }
        tmp[i] = m_sTorrent[m_nIndex];
        ++m_nIndex;
        ++i;
        if(i > MAX_INT_SIZE)
        {
            return ERR_CODE;
        }
    }
    int nStartPos = ++m_nIndex;
    int nStrLen = atoi(tmp);
    if(0 == nStrLen)
    {
        return ERR_CODE;
    }
    
    m_nIndex += nStrLen;
    if(NULL == strOut)
    {
        return output(m_sTorrent+nStartPos, nStrLen);
    }
    else
    {
        memcpy(strOut, m_sTorrent+nStartPos, nStrLen);
        strOut[nStrLen] = '\0';
    }

    return SUCCESS;
}

//l<bencoding±àÂëÀàÐÍ>e  
int CBTParser::parseList()
{
    int iRet = SUCCESS;
    if(m_sTorrent[m_nIndex++] != 'l')
    {
        return ERR_CODE;
    }
    while('e' != m_sTorrent[m_nIndex])
    {
        if(SUCCESS != (iRet = parseBencode()))
        {
            break;
        }
        if('e' != m_sTorrent[m_nIndex])
        {
            if(SUCCESS != (iRet = output(" ")))
            {
                break;
            }
        }
    }
    ++m_nIndex;
    return iRet;
}
int CBTParser::outputBegin(char* str)
{
#ifdef MYDEBUG
    printf("\n<%s>", str);    
#endif
    fprintf(m_pFile, "\n<%s>", str);  
    return SUCCESS;
}

int CBTParser::output(char* str)
{
#ifdef MYDEBUG
    printf("%s", str);
#endif
    fprintf(m_pFile, "%s", str);  
    return SUCCESS;
}

int CBTParser::output(char* str, int strLen)
{
#ifdef MYDEBUG
    int i = 0;
    for(; i < strLen; ++i)
    {
        printf("%c", str[i]);
    }
#endif
    fwrite(str, 1, strLen, m_pFile);
    return SUCCESS;
}
int CBTParser::outputEnd(char* str)
{
#ifdef MYDEBUG
    printf("</%s>", str);
#endif
    fprintf(m_pFile, "</%s>", str);
    return SUCCESS;
}

