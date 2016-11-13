#pragma once
#include "stdafx.h"
class _CABencodeInteger :
	public _CABencode
{
public:
	_CABencodeInteger();
	~_CABencodeInteger();
	long long _value;
	virtual BencodeType GetType();
	virtual BencodeType Parse(_CAFileStream & fileStream);
	virtual void Output(const std::wstring & fileName, const int & layer);
};

