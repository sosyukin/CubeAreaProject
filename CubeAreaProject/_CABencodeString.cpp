#include "stdafx.h"
#include "_CABencodeString.h"



_CABencodeString::_CABencodeString(_CAFileStream & fileStream)
{
	Parse(fileStream);
}

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
	// Parse integer
	std::stringstream stringstream;
	while (fileStream.CurrentByteIsNumber())
	{
		stringstream << *fileStream._current;
		fileStream++;
	}
	//int stringLength;
	long long stringLength;
	stringstream >> stringLength;

	// Check & Skip letter ':'
	if (*fileStream._current != ':')
		throw std::exception("[ERROR] Check letter ':'.");
	fileStream++;

	// Get content.
	_string.assign(fileStream._current, stringLength);
	fileStream += stringLength;

	return BencodeType::BenString;
}

void _CABencodeString::Output(const std::wstring & fileName, const int & layer)
{
	_CABencode::Output(fileName, layer);
	_CALog::Log(std::string(_string).append("\n"), fileName);
}
