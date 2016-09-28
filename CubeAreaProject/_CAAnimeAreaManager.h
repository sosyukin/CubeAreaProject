#pragma once
#include "stdafx.h"
#include "_CAAreaManager.h"

class _CAAnimeAreaManager :
	public _CAAreaManager
{
public:
	_CAAnimeAreaManager();
	~_CAAnimeAreaManager();
	virtual bool Register(wchar_t * filePath);
	// Analyze Anime
	virtual bool Analyze(_CAObject & anime);
};

