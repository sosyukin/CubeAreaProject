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
		stringstream << *fileStream._Current;
		fileStream++;
	}
	int stringLength;
	stringstream >> stringLength;
	if (*fileStream._Current != ':')
		throw std::exception("Can not parse : in StringParse.");
	fileStream++;
	_string.assign(fileStream._Current, stringLength);
	fileStream += stringLength;
	return BencodeType::BenString;
}

void _CABencodeString::Output(const int & layer)
{
	_CABencode::Output(layer);
	//std::wstring a = _CACodeLab::UTF82WChar(_string);
	std::cout << "<string> " << _string << std::endl;
}
