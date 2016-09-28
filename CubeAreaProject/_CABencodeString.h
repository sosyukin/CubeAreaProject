#pragma once
#include "stdafx.h"
class _CABencodeString :
	public _CABencode
{
public:
	_CABencodeString();
	~_CABencodeString();
	std::string _string;
	virtual BencodeType GetType();
	virtual BencodeType Parse(_CAFileStream & fileStream);
	virtual void Output(const int & layer);
};

