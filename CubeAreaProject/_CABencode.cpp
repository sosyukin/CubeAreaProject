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
	// TODO: 在此处插入 return 语句
}

void _CABencode::Output(const int & layer)
{
	for (int i = 0; i < layer; i++)
	{
		std::cout << " ";
	}
	
}
