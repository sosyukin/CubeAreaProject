#pragma once
#include "stdafx.h"
#include "_CAObject.h"
class _CAAnime :
	public _CAObject
{
public:
	_CAAnime(_CAFile* animeFile);
	~_CAAnime();

	bool Split(const std::wstring & srcString, std::vector<std::wstring>& subStrList);

	_CAFile * _pAnimeFile;
	std::vector<std::wstring> _subStrList;
};

