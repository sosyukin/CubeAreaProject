#pragma once
class _CALog
{
public:
	_CALog();
	~_CALog();
	static void SetLogFile(const std::wstring & fileName);
	static friend _CALog & operator<<(_CALog & log, const std::wstring & logString);
private:
	static std::wstring _logFile;
	static std::wstring _encoding;
	static bool _setLogFile;
};

_CALog & operator<<(_CALog & log, const std::wstring & logString);