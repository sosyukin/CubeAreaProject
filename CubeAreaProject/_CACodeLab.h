#pragma once
#include "stdafx.h"

class _CACodeLab
{
	typedef unsigned char BYTE;
public:
	_CACodeLab();
	~_CACodeLab();
	void GetCurrentWorkDirectory();
	void WriteFileOnLength();
	void ReadFileWithBuffer();
	static bool GetFileLength(size_t & filelength, const std::wstring & filename);
	static size_t GetFileLength(const std::wstring & filePath);
	void WriteFileWithMemMapping();
	void CountTime();
	static bool ReadFileWithMemMapping(std::vector<BYTE>& filestream, const std::wstring & filename);
	static bool ReadFileWithMemMapping(BYTE * filestream, const size_t & fileLength, const std::wstring & filename);
	static bool ReadFileWithMemMapping(std::string & filestream, const size_t & fileLength, const std::wstring & filename);
	//static bool ReadFileWithMemMapping(_CAFileStream filestream, const INT64 & fileLength, const std::wstring & filename);
	// Get file size.
	static __int64 CLGetFileSize(std::wstring filePath);
	void GetDirList();
	// Test virtual function.
	void TestVirtualFunction();
	//Converting a WChar string to a Ansi string
	//static std::string WChar2Ansi(LPCWSTR pwszSrc);
	//Converting a Ansi string to WChar string 
	//static std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
	//static std::wstring Str2WStr(const std::string & src);
	//static std::string WStringToMBytes(const wchar_t * lpwcszWString);
	// Output info to file.
	//static void FileOut(std::wstring str, std::wstring filename);
	static int THREAD_MAX_NUM;
	static int thread_mutex;
	static std::mutex threadNumMutex;
	static std::mutex fileMutex;
	static std::vector<std::thread> ThreadList;
	//static std::wstring UTF82WChar(std::string utf8Str);
	static bool EscapeSequence(std::wstring & str);
	static void CAMoveFile(const std::wstring & srcPath, const std::wstring & destPath);
	static bool CAMoveFileAPI(const std::wstring & srcPath, const std::wstring & destPath);
	static void CACopyFile(const std::wstring & srcPath, const std::wstring & destPath);
	static void Rename(const std::wstring & srcPath, const std::wstring & destPath);
	static void DacapoCheck(const std::wstring & srcPath, const std::wstring & destPath);
	static bool SameStr(const std::wstring & str1, const std::wstring & str2);
	static bool StrMatch(const wchar_t & str1, const wchar_t & str2);
	static double StrSimilarity(const std::wstring & str1, const std::wstring & str2);
	static double CharSimilarity(const wchar_t & str1, const wchar_t & str2);
	static bool IsCharacter(const wchar_t & str1, const wchar_t & str2);
	static bool IsNumber(const wchar_t & str1, const wchar_t & str2);
	static bool IsSymbol(const wchar_t & str1, const wchar_t & str2);
};

