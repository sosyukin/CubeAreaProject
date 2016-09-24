#pragma once
#include "_CAFileStream.h"
#include "_CABencode.h"
#include "_CABencodeInteger.h"
#include "_CABencodeString.h"
#include "_CABencodeList.h"
#include "_CABencodeDictionaries.h"

class _CABencodeParser
{
public:
	_CABencodeParser();
	~_CABencodeParser();
	static _CABencode * Parse(const _CAFileStream & fileStream);
	static _CABencode::BencodeType ParseType(const _CAFileStream & fileStream);
};

