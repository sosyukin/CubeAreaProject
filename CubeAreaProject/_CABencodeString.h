#pragma once
#include <sstream>
#include <string>
#include "_CACodeLab.h"
#include "_CABencode.h"

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

