#include "stdafx.h"
#include "_CABencodeParser.h"



_CABencodeParser::_CABencodeParser()
{
}


_CABencodeParser::~_CABencodeParser()
{
}


_CABencode * _CABencodeParser::Detect(const _CAFileStream & fileStream)
{
	switch (*fileStream._current)
	{
	case 'i':
		return new _CABencodeInteger();
	case 'l':
		return new _CABencodeList();
	case 'd':
		return new _CABencodeDictionary();
	default:
		break;
	}
	if (fileStream.CurrentByteIsNumber())
		return new _CABencodeString();
	return NULL;
}

_CABencode::BencodeType _CABencodeParser::ParseType(const _CAFileStream & fileStream)
{
	switch (*fileStream._current)
	{
	case 'i':
		return _CABencode::BencodeType::BenInteger;
	case 'l':
		return _CABencode::BencodeType::BenList;
	case 'd':
		return _CABencode::BencodeType::BenDictionary;
	default:
		break;
	}
	if (fileStream.CurrentByteIsNumber())
		return _CABencode::BencodeType::BenString;
	return _CABencode::BencodeType::BenNull;
}
