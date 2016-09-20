#pragma once
#include <iostream>
#include "_CAAreaManager.h"
#include "_CAObject.h"
#include "_CAAnime.h"
#include "_CAFile.h"

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

