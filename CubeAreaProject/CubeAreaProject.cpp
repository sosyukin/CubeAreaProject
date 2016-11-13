#include "stdafx.h"
// CubeAreaProject.cpp : 定义控制台应用程序的入口点。
//

void thread1()
{
	std::wstring CubeAreaFolder(L"D:\\BTTest\\Torrent");
	_CAFolder folder(CubeAreaFolder);
}
void thread2()
{
	std::wstring CubeAreaFolder(L"E:\\CubeArea");
	_CAFolder folder(CubeAreaFolder);
}
int main()
{
	//_CATestSpace::EscapeSequence();
	//std::wstring DownloadFolder(L"Z:\\shared_x9\\921010321(3949256)\\TDDOWNLOAD");
	/*std::thread t1(thread1);
	std::thread t2(thread2);
	t1.join();
	t2.join();*/
	/*std::wstring CubeAreaFolder(L"D:\\CubeArea");
	_CAFolder folder(CubeAreaFolder);
	std::wstring CubeAreaFolder1(L"E:\\CubeArea");
	_CAFolder folder1(CubeAreaFolder1);*/

	
	//bool state = CreateDirectory(L"D:\\myfolder", NULL);
	//_CAFolder folder(L"D:\\myfolder");
	//_CATestSpace::TestSQLServerConnection();
	//_CATestSpace::TestDB();
	//_CATestSpace::TestFile();
	//_CAFolder folder(L"E:\\CubeArea");
	//_CATestSpace::TestBTParser();
	//_CATestSpace::TestBTChecker();
	//_CATestSpace::TestGetFileLength();
	//_CATestSpace::TestFileStream();
	//_CATestSpace::TestReadFileWithMemMapping();
	//_CATestSpace::TestBencodeInteger();
	//_CATestSpace::TestBencodeString();
	//_CATestSpace::TestBencodeList();
	//_CATestSpace::TestBencodeDictionary();
	//_CATestSpace::TestFullPermutation();
	//_CATestSpace::TestDataBlock();
	
	//_CATestSpace::TestRegex();
	//_CATestSpace::TestLog();
	//_CATestSpace::TestCharConversion();
	//thread1();
	_CATestSpace::TestTorrent();
	/*
	try
	{
		//_CACubeArea CubeArea(L"D:\\CubeArea");
		
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	*/
	return 0;
}

