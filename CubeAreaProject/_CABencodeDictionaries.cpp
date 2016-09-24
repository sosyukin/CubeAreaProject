#include "_CABencodeDictionaries.h"



_CABencodeDictionaries::_CABencodeDictionaries()
{
}


_CABencodeDictionaries::~_CABencodeDictionaries()
{
	for (std::map<std::string, _CABencode *>::iterator i = _dictionaries.begin(); i != _dictionaries.end(); i++)
	{
		delete[] i->second;
	}
}


_CABencode::BencodeType _CABencodeDictionaries::GetType()
{
	return _CABencode::BencodeType::BenDictionaries;
}


_CABencode::BencodeType _CABencodeDictionaries::Parse(_CAFileStream & fileStream)
{
	fileStream++;
	while (*fileStream._Current != 'e')
	{
		if (_CABencodeParser::ParseType(fileStream) != _CABencode::BencodeType::BenString)
			throw std::exception("Not found dict string in Directionaries.");
		_CABencodeString bencodeString;
		bencodeString.Parse(fileStream);
		_CABencode * pBencode = _CABencodeParser::Parse(fileStream);
		if (!pBencode)
			throw std::exception("Find unknown Bencode in BencodeDictionaries.");
		pBencode->Parse(fileStream);
		_dictionaries.insert(std::pair<std::string, _CABencode *>(bencodeString._string, pBencode));
	}
	fileStream++;
	return _CABencode::BencodeType::BenDictionaries;
}

void _CABencodeDictionaries::Output(const int & layer)
{
	for (std::map<std::string, _CABencode *>::iterator i = _dictionaries.begin(); i != _dictionaries.end(); i++)
	{
		_CABencode::Output(layer);
		std::cout << "[" << i->first << "]\n";
		i->second->Output(layer + 1);
	}
}
