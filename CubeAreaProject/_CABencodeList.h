#pragma once
#include <list>
#include "_CABencode.h"
#include "_CABencodeParser.h"
class _CABencodeList :
	public _CABencode
{
public:
	_CABencodeList();
	~_CABencodeList();
	std::list<_CABencode *> _bencodeList;
	virtual BencodeType GetType();
	_CABencode::BencodeType Parse(_CAFileStream & fileStream);
	virtual void Output(const int & layer);
};

