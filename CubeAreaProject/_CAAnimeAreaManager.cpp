#include "stdafx.h"
#include "_CAAnimeAreaManager.h"



_CAAnimeAreaManager::_CAAnimeAreaManager()
{
	// TODO : Emergency
	// CAFolder created CAFile will deleted when CAFolder has been destoryed.
	_CAFolder AnimeAreaFolder(L"D:\\Anime");
	_CAFolder SearchFolder(L"D:\\Rebuild");
	std::vector<_CAFile *> fileList;
	SearchFolder.GetFileList(L"mp4", fileList, _CACodeLab::SameStr, _CAFile::ATTR::SUFFIX);
	std::vector<std::wstring> strLib;
	for (auto file : fileList)
	{
		std::vector<std::wstring> subStrList;
		if (Split(file->Name().substr(0, file->Name().find_last_of('.')), subStrList))
		{
			//std::cerr << _CACharConversion::unicode2ansi(file->Name().substr(0, file->Name().find_last_of('.'))) << " == ";
			for (auto i : subStrList)
			{
				//std::cerr << _CACharConversion::unicode2ansi(i) << " ";
				//std::transform(i.begin(), i.end(), i.begin(), tolower);
				//if (std::find(strLib.begin(), strLib.end(), i) == strLib.end())
				//{
				//	strLib.push_back(i);
				//}
				if (std::find(strLib.begin(), strLib.end(), i) == strLib.end())
					strLib.push_back(i);
				for (auto strInLib : strLib)
				{
					if (_CACodeLab::StrSimilarity(i, strInLib) > 0.2 && _CACodeLab::StrSimilarity(i, strInLib) != 1)
					{
						std::cerr << _CACharConversion::unicode2ansi(i) << " with [ " << _CACharConversion::unicode2ansi(strInLib) << " ] == " << _CACodeLab::StrSimilarity(i, strInLib) <<  std::endl;
					}
					
					

				}
			}
			//std::cerr << std::endl;
		}
		//else std::cerr << _CACharConversion::unicode2ansi(file->Name().substr(0, file->Name().find_last_of('.'))) << " ERROR" << std::endl;
	}
	//for (auto str : strLib)
	//{
	//	std::cerr << _CACharConversion::unicode2ansi(str) << std::endl;
	//}
}


_CAAnimeAreaManager::~_CAAnimeAreaManager()
{
}


bool _CAAnimeAreaManager::Register(wchar_t * filePath)
{
	try
	{
		//_CAAnime anime;
		_CAFile file(filePath);
		std::wcout << L"FileName = " << file.Name() << std::endl;
		std::wcout << L"FileSuffix = " << file.Suffix() << std::endl;
		std::wcout << L"FileSize = " << file.Size() << std::endl;
		//this->Analyze(&anime);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	std::wcout << filePath << std::endl;
	return false;
}


// Analyze Anime
bool _CAAnimeAreaManager::Analyze(_CAObject * anime)
{
	return false;
}

bool _CAAnimeAreaManager::Split(const std::wstring & srcString, std::vector<std::wstring>& subStrList)
{
	// Set StrBegin at begin of string.
	std::wstring::const_iterator subStrBegin = srcString.cbegin();
	// Find subStr if StrBegin not end.
	while (subStrBegin != srcString.cend())
	{
		// Set StrEnd at next of StrBegin.
		std::wstring::const_iterator subStrEnd = subStrBegin + 1;
		while (true)
		{
			// StrEnd is end of str?
			if (subStrEnd == srcString.cend())
				return false;
			// StrEnd is match ?
			if (_CACodeLab::StrMatch(*subStrBegin, *subStrEnd))
			{
				// Matched
				std::wstring::const_iterator found = std::find(subStrBegin + 1, subStrEnd, L'&');
				if (found != subStrEnd)
				{
					subStrList.push_back(std::wstring(subStrBegin + 1, found));
					subStrList.push_back(std::wstring(found + 1, subStrEnd));
				}
				else subStrList.push_back(std::wstring(subStrBegin + 1, subStrEnd));
				subStrBegin = subStrEnd + 1;
				break;
			}
			// Not Matched
			else subStrEnd++;
		}
	}
	return true;
}

bool _CAAnimeAreaManager::Classify(std::vector<_CAFile *> & fileList)
{
	for (auto pFile : fileList)
	{
		_CAAnime anime(pFile);
	}
	return false;
}

