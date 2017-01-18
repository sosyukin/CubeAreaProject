#pragma once
#include "stdafx.h"
class _CAAreaManager
{
public:
	_CAAreaManager();
	~_CAAreaManager();
	// Register works
	virtual bool Register(wchar_t * filePath) = 0;
	// analyze object
	virtual bool Analyze(_CAObject * object) = 0;
};

