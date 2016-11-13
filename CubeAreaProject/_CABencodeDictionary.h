#pragma once
#include "stdafx.h"

class _CABencodeDictionary :
	public _CABencode
{
public:
	std::map<std::string, _CABencode *> _dictionary;
	_CABencodeDictionary();
	~_CABencodeDictionary();
	virtual _CABencode::BencodeType GetType();
	virtual _CABencode::BencodeType Parse(_CAFileStream & fileStream);
	void Output(const std::wstring & fileName, const int & layer);
	bool Find(const std::string & dict);
};

