#include "stdafx.h"
// CubeAreaProject.cpp : �������̨Ӧ�ó������ڵ㡣
//

int main()
{
	//_CATestSpace::TestFile();
	//_CAFolder folder(L"E:\\CubeArea");
	//_CATestSpace::TestBTParser();
	_CATestSpace::TestTorrent();
	try
	{
		//_CATestSpace::TestBTChecker();
		//_CATestSpace::TestGetFileLength();
		//_CATestSpace::TestFileStream();
		//_CATestSpace::TestReadFileWithMemMapping();
		//_CATestSpace::TestBencodeInteger();
		//_CATestSpace::TestBencodeString();
		//_CATestSpace::TestBencodeList();
		//_CATestSpace::TestBencodeDictionaries();
		//_CATestSpace::TestFullPermutation();
		//_CATestSpace::TestDataBlock();
		
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}

