#include "stdafx.h"
// CubeAreaProject.cpp : �������̨Ӧ�ó������ڵ㡣
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

