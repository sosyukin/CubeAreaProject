#pragma once
#include "_CAAreaBase.h"
#include "_CAAnimeArea.h"
class _CACubeArea
	: public _CAAreaBase
{
	std::vector<_CAAreaBase> _areaList;
public:
	_CACubeArea();
	_CACubeArea(const std::wstring & filePath);
	~_CACubeArea();
};

