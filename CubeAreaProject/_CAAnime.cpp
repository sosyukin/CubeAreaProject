#include "stdafx.h"
#include "_CAAnime.h"


_CAAnime::_CAAnime(_CAFile * animeFile)
	: _pAnimeFile(animeFile)
{
	Split(_pAnimeFile->Name().substr(0, _pAnimeFile->Name().find_last_of('.')), _subStrList);
}

_CAAnime::~_CAAnime()
{
}

bool _CAAnime::Split(const std::wstring & srcString, std::vector<std::wstring>& subStrList)
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