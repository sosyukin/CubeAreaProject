#pragma once
#include "_CAObject.h"
#include "_CAFolder.h"
class _CAAreaBase :
	public _CAObject
{
	_CAFolder AreaRootFolder;
public:
	_CAAreaBase();
	_CAAreaBase(const std::wstring & filePath);
	~_CAAreaBase();
};

