#pragma once
#include "stdafx.h"
class _CABencodeParser
{
public:
	_CABencodeParser();
	~_CABencodeParser();
	static _CABencode * Parse(const _CAFileStream & fileStream);
	static _CABencode::BencodeType ParseType(const _CAFileStream & fileStream);
};

