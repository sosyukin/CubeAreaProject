#include "stdafx.h"
#include "_CABencode.h"



_CABencode::_CABencode()
{
}


_CABencode::~_CABencode()
{
}


// Check type of Bencode
_CABencode::BencodeType _CABencode::Parse(_CAFileStream & fileStream)
{
	return BencodeType::BenNull;
	// TODO: �ڴ˴����� return ���
}

void _CABencode::Output(const std::wstring & fileName, const int & layer)
{
	for (int i = 0; i < layer; i++)
	{
		_CALog::Log(" ", fileName);
	}
}