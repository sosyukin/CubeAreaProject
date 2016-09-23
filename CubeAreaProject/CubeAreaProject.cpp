// CubeAreaProject.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>

#include "_CACodeLab.h"
#include "_CAAnimeAreaManager.h"
#include "_CAFile.h"
#include "_CAFolder.h"
#include "_CAMD5.h"
#include "SHA1.h"
#include "BTParser.h"
#include "_CATestSpace.h"


int main()
{
	//_CAFolder folder(L"E:\\CubeArea");
	//_CATestSpace::TestBTChecker();
	//_CATestSpace::TestGetFileLength();
	//_CATestSpace::TestFileStream();
	_CATestSpace::TestReadFileWithMemMapping();
	//_CATestSpace::TestBTParser();
	return 0;
}

