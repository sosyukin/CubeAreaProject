#pragma once
class _CALog
{
public:
	_CALog();
	~_CALog();
	//static void SetLogFile(const std::wstring & fileName);
	static void Log(const std::wstring & logStr, const std::wstring & logFile);
	static void Log(const std::string & logStr, const std::wstring & logFile);
	static void Log(const std::wstring & logStr);
	static void Log(const std::string & logStr);
private:
	//static std::wstring _logFile;
	static std::wstring _encoding;
	//static bool _setLogFile;
};