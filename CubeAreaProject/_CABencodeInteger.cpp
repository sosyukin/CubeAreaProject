#include "_CABencodeInteger.h"
#include <sstream>


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
		stringstream << *fileStream._Current;
		fileStream++;
	}
	if (*fileStream._Current == 'e')
	{
		stringstream >> _value;
		fileStream++;
	}
	else
		throw std::exception("Parse a non-number or e in IntegerParse.");
	return BencodeType::BenInteger;
}
