#include "stdafx.h"
#include "_CACharConversion.h"


_CACharConversion::_CACharConversion()
{
}


_CACharConversion::~_CACharConversion()
{
}

std::wstring _CACharConversion::utf82unicode(const std::string & utf8)
{
	if (utf8.empty())
		return std::wstring();
	if (utf8.length() > (std::numeric_limits<int>::max)())
	{
		// TODO : Send error.
		//std::cout << "String(utf8) too long." << std::endl;
		return std::wstring();
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	wchar_t * pwText = new wchar_t[dwUnicodeLen];
	memset(pwText, 0, dwUnicodeLen);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, pwText, dwUnicodeLen);
	std::wstring unicode(pwText, dwUnicodeLen - 1);
	delete[] pwText;
	return unicode;
}

std::wstring _CACharConversion::ansi2unicode(const std::string & ansi)
{
	if (ansi.empty())
		return std::wstring();
	if (ansi.length() > (std::numeric_limits<int>::max)())
	{
		// TODO : Send error.
		return std::wstring();
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, NULL, 0);
	wchar_t * pwText = new wchar_t[dwUnicodeLen];
	memset(pwText, 0, dwUnicodeLen);
	MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, pwText, dwUnicodeLen);
	std::wstring unicode(pwText, dwUnicodeLen - 1);
	delete[] pwText;
	return unicode;
}

std::string _CACharConversion::unicode2utf8(const std::wstring & unicode)
{
	if (unicode.empty())
		return std::string();
	if (unicode.length() > (std::numeric_limits<int>::max)())
	{
		// TODO : Send error.
		return std::string();
	}
	int dwUtf8Len = WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);
	char * pText = new char[dwUtf8Len];
	memset(pText, 0, dwUtf8Len);
	WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, pText, dwUtf8Len, NULL, NULL);
	std::string utf8(pText, dwUtf8Len - 1);
	delete[] pText;
	return utf8;
}

std::string _CACharConversion::unicode2ansi(const std::wstring & unicode)
{
	if (unicode.empty())
		return std::string();
	if (unicode.length() > (std::numeric_limits<int>::max)())
	{
		// TODO : Send error.
		return std::string();
	}
	int dwAnsiLen = WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);
	char * pText = new char[dwAnsiLen];
	memset(pText, 0, dwAnsiLen);
	WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), -1, pText, dwAnsiLen, NULL, NULL);
	std::string Ansi(pText, dwAnsiLen - 1);
	delete[] pText;
	return Ansi;
}

