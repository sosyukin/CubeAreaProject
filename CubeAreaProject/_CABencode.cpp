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