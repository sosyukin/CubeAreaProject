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
	virtual bool Analyze(_CAObject * anime);
	bool Check();
	bool Split(const std::wstring & srcString, std::vector<std::wstring> & subStrList);

	/// <summary>
	/// <Function Name>
	/// Classify Anime
	/// </Function Name>
	/// <Flow Control>
	/// 1. Analyze a anime file
	///    return the information
	///    see <Analyze> function
	/// 2. Check if anime folder exist
	///    return true if exist
	///    return true if not exist but create a new folder
	///    return false if can not create folder
	///    see <Check> function
	/// 3. Move the anime file to target folder
	///    return true if move succeed, otherwise return false
	/// </Flow Control>
	/// </summary>
	bool Classify(std::vector<_CAFile *> & fileList);
};

