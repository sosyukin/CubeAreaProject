#include "stdafx.h"
#include "_CABencodeString.h"



_CABencodeString::_CABencodeString()
{
}


_CABencodeString::~_CABencodeString()
{
}


_CABencode::BencodeType _CABencodeString::GetType()
{
	return BencodeType::BenString;
}


_CABencode::BencodeType _CABencodeString::Parse(_CAFileStream & fileStream)
{
	std::stringstream stringstream;
	while (fileStream.CurrentByteIsNumber())
	{
		stringstream << *fileStream._current;
		fileStream++;
	}
	int stringLength;
	stringstream >> stringLength;
	if (*fileStream._current != ':')
		throw std::exception("Can not parse : in StringParse.");
	fileStream++;
	_string.assign(fileStream._current, stringLength);
	fileStream += stringLength;
	return BencodeType::BenString;
}

void _CABencodeString::Output(const int & layer)
{
	_CABencode::Output(layer);
	//std::wstring a = _CACharConversion::utf82unicode(_string);
	std::cout << "<string> " << _string << std::endl;
}
