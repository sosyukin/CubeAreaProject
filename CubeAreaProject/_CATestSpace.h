#pragma once
#include <string>
#include <mutex>
#include "_CAMD5.h"
#include "_CASHA1.h"
class _CATestSpace
{
public:
	_CATestSpace();
	~_CATestSpace();
	// Test _CAMD5
	static void TestMD5();
	static void PrintMD5(const std::string & str, _CAMD5 & md5);
	static std::string FileDigest(const std::string & file);
	// Test Thread
	static void TestThread();
	static void threadfunction(int i);
	static void myThreadA();
	static void myThreadB();
	static void f1(int n);
	static void f2(int & n);
	static int publicvar;
	static int tickets;
	//static std::mutex mymutex;
	// Test CBTParser
	static void TestCBTParser();
	// Test _CABTChecker
	static void TestBTChecker();
	static void TestBTParser();
	// Test GetFileLength
	static void TestGetFileLength();
	// Test ReadFileWithMemMapping
	static void TestReadFileWithMemMapping();
	// Test FileStream and DataBlock
	static void TestFileStream();
	static void TestBencodeInteger();
	static void TestBencodeString();
	static void TestBencodeList();
	static void TestBencodeDictionaries();
};

