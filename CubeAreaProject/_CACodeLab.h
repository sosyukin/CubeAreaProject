#pragma once
#include <direct.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <Windows.h>


#include <vector>
#include <io.h>

//#include "_CAFile.h"
//#include "_CAFolder.h"
class _CACodeLab
{
	typedef unsigned char BYTE;
public:
	_CACodeLab();
	~_CACodeLab();
	void GetCurrentWorkDirectory();
	void WriteFileOnLength();
	void ReadFileWithBuffer();
	static bool GetFileLength(int & filelength, const std::wstring & filename);
	void WriteFileWithMemMapping();
	void CountTime();
	static bool ReadFileWithMemMapping(std::vector<BYTE>& filestream, const std::wstring & filename);
	// Get file size.
	static __int64 CLGetFileSize(std::wstring filePath);
	void GetDirList();
	// Test virtual function.
	void TestVirtualFunction();
	//Converting a WChar string to a Ansi string
	static std::string WChar2Ansi(LPCWSTR pwszSrc);
	//Converting a Ansi string to WChar string 
	static std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
	static std::string WStringToMBytes(const wchar_t * lpwcszWString);
	// Output info to file.
	static void FileOut(std::wstring str, std::wstring filename);
	static int THREAD_MAX_NUM;
	static int thread_mutex;
	static std::mutex threadNumMutex;
	static std::mutex fileMutex;
	static std::vector<std::thread> ThreadList;
};

