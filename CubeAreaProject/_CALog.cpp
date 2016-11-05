#include "stdafx.h"
#include "_CALog.h"

std::wstring _CALog::_encoding(L"UTF-8");
std::wstring _CALog::_logFile;
bool _CALog::_setLogFile = false;

_CALog::_CALog()
{
}


_CALog::~_CALog()
{
}

void _CALog::SetLogFile(const std::wstring & fileName)
{
	_setLogFile = true;
	_logFile = fileName;
}

_CALog & operator<<(_CALog & log, const std::wstring & logString)
{

	return log;
}
