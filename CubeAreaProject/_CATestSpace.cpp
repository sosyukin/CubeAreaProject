#include "_CATestSpace.h"
#include <iostream>
#include <fstream>
#include <string>

#include <thread>
#include <mutex>
#include <Windows.h>
#include <sstream>

#include "_CACodeLab.h"
#include "_CAMD5.h"
#include "BTParser.h"
#include "_CABTChecker.h"
#include "_CABTParser.h"
#include "SHA1.h"
#include "_CAFileStream.h"

_CATestSpace::_CATestSpace()
{
}


_CATestSpace::~_CATestSpace()
{
}


// Test _CAMD5
void _CATestSpace::TestMD5()
{
	std::cout << _CAMD5("abc").toString() << std::endl;
	//std::cout << _CAMD5(std::ifstream("D:\\test.txt")).toString() << std::endl;
	//std::cout << _CAMD5(ifstream("D:\\test.exe", std::ios::binary)).toString() << std::endl;
	std::cout << FileDigest("D:\\test.exe") << std::endl;
	_CAMD5 md5;
	md5.update("");
	PrintMD5("", md5);

	md5.update("a");
	PrintMD5("a", md5);

	md5.update("bc");
	PrintMD5("abc", md5);

	md5.update("defghijklmnopqrstuvwxyz");
	PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);

	md5.reset();
	md5.update("message digest");
	PrintMD5("message digest", md5);

	md5.reset();
	md5.update(ifstream("D:\\test.txt"));
	PrintMD5("D:\\test.txt", md5);

}


void _CATestSpace::PrintMD5(const std::string & str, _CAMD5 & md5)
{
	std::cout << "MD5(\"" << str << "\") = " << md5.toString() << std::endl;
}


std::string _CATestSpace::FileDigest(const string & file)
{
	std::ifstream in(file.c_str(), std::ios::binary);
	if (!in)
	{
		return "";
	}
	_CAMD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof())
	{
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
		{
			md5.update(buffer, length);
		}
	}
	in.close();
	return md5.toString();
}

// Test Thread
void _CATestSpace::TestThread()
{
	int n = 0;
	std::thread t1;
	std::thread t2(f1, n + 1);
	std::thread t3(f2, std::ref(n));
	std::thread t4(std::move(t3));
	std::thread t5(f1, n + 1);
	std::thread t6(f1, n + 1);
	std::thread t7(f1, n + 1);
	std::thread t8(f1, n + 1);
	std::thread t9(f1, n + 1);
	t2.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	f1(n);
	std::cout << "Final value of n is " << n << "\n";
	//std::thread t1(&threadfunction, 1);
	//std::thread t2(&threadfunction, 2);
	//std::thread t3(&threadfunction, 3);
	//std::thread t4(&threadfunction, 4);
	//std::thread t5(&threadfunction, 5);
	//std::thread t6(&threadfunction, 6);
	//std::thread t7(&threadfunction, 7);
	//std::thread t8(&threadfunction, 8);
	//std::thread t9(&threadfunction, 9);
	std::thread tA(&myThreadA);//创建一个分支线程，回调到myThread函数里
	std::thread tB(&myThreadB);
	tA.join();
	tB.join();
	/*system("dir");
	while (!std::cin.eof())
	{
	std::cout << "cached " << std::cin.get() << std::endl;
	}*/
	/*t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();*/
}
int _CATestSpace::publicvar = 100;
int _CATestSpace::tickets = 100;
void _CATestSpace::threadfunction(int i)
{
	//printf("Hello world!\n");
	Sleep(1000);
	publicvar = publicvar + 10;
	std::cout << publicvar << " Hello world!\n";

}
void _CATestSpace::myThreadA()
{
	while (true)
	{
		//mymutex.lock();
		if (tickets>0)
		{
			Sleep(10);
			std::cout << tickets-- << std::endl;
			//CCLOG("A Sell %d", tickets--);//输出售票，每次减1
			//mymutex.unlock();
		}
		else {
			//mymutex.unlock();
			break;

		}
	}
}
void _CATestSpace::myThreadB()
{
	while (true)
	{
		//mymutex.lock();
		if (tickets>0)
		{
			Sleep(10);
			printf("%d\n", tickets--);
			//mymutex.unlock();
			//std::cout << tickets-- << std::endl;
			//CCLOG("B Sell %d", tickets--);
		}
		else
		{
			//mymutex.unlock();
			break;

		}
	}
}
void _CATestSpace::f1(int n)
{
	for (size_t i = 0; i < 500000; i++)
	{
		for (size_t i = 0; i < 500000; i++)
		{
			sin(i);
			//n = i;
		}

		//std::cout << "Thread " << n << " executing\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void _CATestSpace::f2(int& n)
{
	for (size_t i = 0; i < 500000; i++)
	{
		for (size_t i = 0; i < 500000; i++)
		{
			sin(i);
		}
		//std::cout << "Thread 2 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

// Test CBTParser
void _CATestSpace::TestCBTParser()
{
	CBTParser parser;
	parser.parse("D:\\A596797E49B21B47883AB7016C33820327AE4F1E.torrent", "D:\\a.xml");
}


// Test _CABTChecker
void _CATestSpace::TestBTChecker()
{
	std::cout << "In BTChecker Testing..." << std::endl;
	_CABTChecker BTChecker;
}


void _CATestSpace::TestBTParser()
{
	std::cout << "In TestBTParser Testing..." << std::endl;
	_CABTParser BTParser;
	//int filelength;
	//_CACodeLab::GetFileLength(filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4.torrent");
	//std::cout << "File size is " << filelength << std::endl;
	BTParser.Open(L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4.torrent");
	BTParser.Check();
}


// Test GetFileLength
void _CATestSpace::TestGetFileLength()
{
	INT64 filelength;
	std::cout << sizeof(filelength) << std::endl;
	std::cout << sizeof(DWORD) << std::endl;
	std::cout << sizeof(LPDWORD) << std::endl;
	std::cout << sizeof(long) << std::endl;
	std::cout << sizeof(long long) << std::endl;
	//_CACodeLab::GetFileLength(filelength, L"d:\\cn_windows_server_2012_r2_with_update_x64_dvd_6052725.iso");
	_CACodeLab::GetFileLength(filelength, L"d:\\cn_windows_server_2012_r2_with_update_x64_dvd_6052725.iso");
	std::cout << filelength << std::endl;
}


// Test ReadFileWithMemMapping
void _CATestSpace::TestReadFileWithMemMapping()
{
	INT64 filelength;
	CSHA1 sha1;
	_CACodeLab::GetFileLength(filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	/* ----- ReadFileWithMemMapping -----
	   -----       Version 2.0      -----
	   ---------------------------------- */
	BYTE * fileStream;
	std::wstring strOut;
	fileStream = new BYTE[filelength];
	_CACodeLab::ReadFileWithMemMapping(fileStream, filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	sha1.Update(fileStream, filelength);
	sha1.Final();
	sha1.ReportHashStl(strOut, CSHA1::REPORT_HEX_SHORT);
	std::wcout << strOut << L" ";
	delete[] fileStream;
	/* ----- ReadFileWithMemMapping -----
	   -----    Version 2.1(beta)   -----
	   -------------ERROR---------------- */
	sha1.Reset();
	_CAFileStream filestream;
	//std::string filestream;
	_CACodeLab::ReadFileWithMemMapping(filestream, filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	sha1.Update((unsigned char *)filestream._Current, filelength);
	sha1.Final();
	sha1.ReportHashStl(strOut, CSHA1::REPORT_HEX_SHORT);
	std::wcout << strOut << std::endl;
}


// Test FileStream and DataBlock
void _CATestSpace::TestFileStream()
{
	_CAFileStream fileStream;
	fileStream.GetData("Hello World!", 13);
	fileStream++;
	fileStream += 2;
}
