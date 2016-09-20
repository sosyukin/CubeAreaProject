#include "_CAAnimeAreaManager.h"



_CAAnimeAreaManager::_CAAnimeAreaManager()
{
}


_CAAnimeAreaManager::~_CAAnimeAreaManager()
{
}


bool _CAAnimeAreaManager::Register(wchar_t * filePath)
{
	try
	{
		_CAAnime anime;
		_CAFile file(filePath);
		std::wcout << L"FileName = " << file.Name() << std::endl;
		std::wcout << L"FileSuffix = " << file.Suffix() << std::endl;
		std::wcout << L"FileSize = " << file.Size() << std::endl;
		this->Analyze(anime);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	std::wcout << filePath << std::endl;
	return false;
}


// Analyze Anime
bool _CAAnimeAreaManager::Analyze(_CAObject & anime)
{
	return false;
}
