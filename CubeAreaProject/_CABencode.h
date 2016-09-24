#pragma once
#include "_CAFileStream.h"
class _CABencode
{
public:
	enum BencodeType
	{
		BenNull,
		BenInteger,
		BenString,
		BenList,
		BenDictionaries
	};
	_CABencode();
	virtual ~_CABencode();
	// Check type of Bencode
	virtual BencodeType Parse(_CAFileStream & fileStream);
	virtual BencodeType GetType() { return BenNull; }
	virtual void Output(const int & layer);
};

