#include "stdafx.h"
// CubeAreaProject.cpp : 定义控制台应用程序的入口点。
//

int main()
{
	try
	{
		_CATestSpace::TestDownloadManager();
		//_CAEnglishGrammarExplanation ege;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

