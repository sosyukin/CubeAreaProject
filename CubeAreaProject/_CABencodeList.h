#pragma once
#include "stdafx.h"
class _CABencodeList :
	public _CABencode
{
public:
	_CABencodeList();
	~_CABencodeList();
	std::list<_CABencode *> _bencodeList;
	virtual BencodeType GetType();
	_CABencode::BencodeType Parse(_CAFileStream & fileStream);
	virtual void Output(const std::wstring & fileName, const int & layer);
};

