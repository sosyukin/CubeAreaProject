#include "stdafx.h"
#include "_CABencodeDictionary.h"



_CABencodeDictionary::_CABencodeDictionary()
{
}


_CABencodeDictionary::~_CABencodeDictionary()
{
	for (std::map<std::string, _CABencode *>::iterator i = _dictionary.begin(); i != _dictionary.end(); i++)
	{
		delete[] i->second;
	}
}


_CABencode::BencodeType _CABencodeDictionary::GetType()
{
	return _CABencode::BencodeType::BenDictionary;
}


_CABencode::BencodeType _CABencodeDictionary::Parse(_CAFileStream & fileStream)
{
	// Skip letter 'd'
	fileStream++;

	// Parse Dictionary until letter 'e'
	while (*fileStream._current != 'e')
	{
		// Parse dictionary string
		if (_CABencodeParser::ParseType(fileStream) != _CABencode::BencodeType::BenString)
			throw std::exception("[ERROR] Parse dictionary string.");
		_CABencodeString bencodeString(fileStream);
		// Detect dictionary content type
		_CABencode * pBencode = _CABencodeParser::Detect(fileStream);
		if (!pBencode)
			throw std::exception("[ERROR] Detect dictionary content type.");
		// Parse dictionary content
		pBencode->Parse(fileStream);

		// Get content.
		_dictionary.insert(std::pair<std::string, _CABencode *>(bencodeString._string, pBencode));
	}

	// Skip letter 'e'
	fileStream++;

	// TODO : remove return
	return _CABencode::BencodeType::BenDictionary;
}

void _CABencodeDictionary::Output(const std::wstring & fileName, const int & layer)
{
	for (std::map<std::string, _CABencode *>::iterator i = _dictionary.begin(); i != _dictionary.end(); i++)
	{
		_CABencode::Output(fileName, layer);
		std::string buf(i->first);
		buf.append("[d]\n");
		_CALog::Log(buf, fileName);
		i->second->Output(fileName, layer + 1);
	}
}

bool _CABencodeDictionary::Find(const std::string & dict)
{
	return _dictionary.find(dict) != _dictionary.end();
}
