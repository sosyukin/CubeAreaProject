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
	// Skip letter 'l'
	fileStream++;

	// Parse list until letter 'e'
	while (*fileStream._current != 'e')
	{
		// Detect list content type
		_CABencode * pBencode = _CABencodeParser::Detect(fileStream);
		if (!pBencode)
			throw std::exception("[ERROR] Detect list content type.");
		// Parse list content
		pBencode->Parse(fileStream);
		// Get content
		_bencodeList.push_back(pBencode);
	}

	// Skip letter 'e'
	fileStream++;

	// TODO : remove return
	return _CABencode::BencodeType::BenList;
}

void _CABencodeList::Output(const std::wstring & fileName, const int & layer)
{
	_CABencode::Output(fileName, layer);
	for (std::list<_CABencode *>::iterator i = _bencodeList.begin(); i != _bencodeList.end(); i++)
	{
		(*i)->Output(fileName, layer + 1);
	}
}
