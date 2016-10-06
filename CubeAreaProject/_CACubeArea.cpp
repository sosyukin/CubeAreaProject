#include "stdafx.h"
#include "_CACubeArea.h"



_CACubeArea::_CACubeArea()
{

}

_CACubeArea::_CACubeArea(const std::wstring & filePath)
	: _CAAreaBase(filePath)
{
	_areaList.push_back(_CAAnimeArea(std::wstring(filePath).append(L"\\AnimeArea")));
}


_CACubeArea::~_CACubeArea()
{
}
