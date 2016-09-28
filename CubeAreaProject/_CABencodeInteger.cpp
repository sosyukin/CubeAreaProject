#include "stdafx.h"
#include "_CABencodeInteger.h"


_CABencodeInteger::_CABencodeInteger()
	: _value(0)
{
}


_CABencodeInteger::~_CABencodeInteger()
{
}


_CABencode::BencodeType _CABencodeInteger::GetType()
{
	return BencodeType::BenInteger;
}


_CABencode::BencodeType _CABencodeInteger::Parse(_CAFileStream & fileStream)
{
	std::stringstream stringstream;
	fileStream++;
	while (fileStream.CurrentByteIsNumber())
	{
		stringstream << *fileStream._current;
		fileStream++;
	}
	if (*fileStream._current == 'e')
	{
		stringstream >> _value;
		fileStream++;
	}
	else
		throw std::exception("Parse a non-number or e in IntegerParse.");
	return BencodeType::BenInteger;
}

void _CABencodeInteger::Output(const int & layer)
{
	_CABencode::Output(layer);
	std::cout << "<Integer> " << _value << std::endl;
}
