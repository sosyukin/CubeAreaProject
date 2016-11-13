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
	// Skip letter 'i'
	fileStream++;

	// Parse integer
	std::stringstream stringstream;
	while (fileStream.CurrentByteIsNumber())
	{
		stringstream << *fileStream._current;
		fileStream++;
	}

	// Check tail letter 'e'
	if (*fileStream._current == 'e')
		stringstream >> _value;
	else
		throw std::exception("[ERROR] Check letter 'e'.");
	
	// Skip letter 'e'
	fileStream++;
	return BencodeType::BenInteger;
}

void _CABencodeInteger::Output(const std::wstring & fileName, const int & layer)
{
	_CABencode::Output(fileName, layer);
	std::stringstream stringstream;
	std::string buf;
	stringstream << _value;
	stringstream >> buf;
	_CALog::Log(std::string(buf).append("\n"), fileName);
}
