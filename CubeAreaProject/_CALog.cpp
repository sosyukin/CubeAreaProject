#include "stdafx.h"
#include "_CALog.h"

std::wstring _CALog::_encoding(L"UTF-8");
//std::wstring _CALog::_logFile;
//bool _CALog::_setLogFile = false;

_CALog::_CALog()
{
}


_CALog::~_CALog()
{
}

//void _CALog::SetLogFile(const std::wstring & fileName)
//{
//	_setLogFile = true;
//	_logFile = fileName;
//}

void _CALog::Log(const std::wstring & logStr, const std::wstring & logFile)
{
	if (_encoding.compare(L"UTF-8") == 0)
	{
		Log(_CACharConversion::unicode2utf8(logStr), logFile);
	}
	else
	{
		Log(_CACharConversion::unicode2ansi(logStr), logFile);
	}
}

void _CALog::Log(const std::string & logStr, const std::wstring & logFile)
{
		std::ofstream fout(logFile, std::ios::app);
		fout << logStr;
		fout.close();
}

void _CALog::Log(const std::wstring & logStr)
{
	if (_encoding.compare(L"UTF-8") == 0)
	{
		Log(_CACharConversion::unicode2utf8(logStr));
	}
	else
	{
		Log(_CACharConversion::unicode2ansi(logStr));
	}
}

void _CALog::Log(const std::string & logStr)
{
	if (_encoding.compare(L"UTF-8") == 0)
	{
		std::cerr << _CACharConversion::unicode2ansi(_CACharConversion::utf82unicode(logStr));
	}
	else
	{
		std::cerr << logStr;
	}
}
