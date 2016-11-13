#pragma once
#include "stdafx.h"
class _CABencode
{
public:
	enum BencodeType
	{
		BenNull,
		BenInteger,
		BenString,
		BenList,
		BenDictionary
	};
	_CABencode();
	virtual ~_CABencode();
	// Check type of Bencode
	virtual BencodeType Parse(_CAFileStream & fileStream);
	virtual BencodeType GetType() { return BenNull; }
	virtual void Output(const std::wstring & fileName, const int & layer);
};

