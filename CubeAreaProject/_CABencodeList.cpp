#include "stdafx.h"
#include "_CABencodeList.h"



_CABencodeList::_CABencodeList()
{
}


_CABencodeList::~_CABencodeList()
{
	for (std::list<_CABencode *>::iterator i = _bencodeList.begin(); i != _bencodeList.end(); i++)
		delete[] *i;
}


_CABencode::BencodeType _CABencodeList::GetType()
{
	return BencodeType::BenList;
}


_CABencode::BencodeType _CABencodeList::Parse(_CAFileStream & fileStream)
{
	fileStream++;
	while (*fileStream._current != 'e')
	{
		_CABencode * pBencode = _CABencodeParser::Parse(fileStream);
		if (!pBencode)
			throw std::exception("Find unknown Bencode in BencodeList.");
		pBencode->Parse(fileStream);
		_bencodeList.push_back(pBencode);
	}
	fileStream++;
	return _CABencode::BencodeType::BenList;
}

void _CABencodeList::Output(const int & layer)
{
	_CABencode::Output(layer);
	std::cout << "<list>" << std::endl;
	for (std::list<_CABencode *>::iterator i = _bencodeList.begin(); i != _bencodeList.end(); i++)
	{
		(*i)->Output(layer + 1);
	}
}
