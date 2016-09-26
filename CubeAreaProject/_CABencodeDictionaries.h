#pragma once
#include <string>
#include <map>

#include "_CABencode.h"
#include "_CABencodeParser.h"
#include "_CABencodeString.h"
#include "_CABencodeInteger.h"
#include "_CABencodeList.h"

class _CABencodeDictionaries :
	public _CABencode
{
public:
	std::map<std::string, _CABencode *> _dictionaries;
	_CABencodeDictionaries();
	~_CABencodeDictionaries();
	virtual _CABencode::BencodeType GetType();
	virtual _CABencode::BencodeType Parse(_CAFileStream & fileStream);
	virtual void Output(const int & layer);
	bool Find(const std::string & dict);
};

