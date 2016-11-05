#pragma once
class _CACharConversion
{
public:
	_CACharConversion();
	~_CACharConversion();
	static std::wstring utf82unicode(const std::string & utf8);
	static std::wstring ansi2unicode(const std::string & ansi);
	static std::string unicode2utf8(const std::wstring & unicode);
	static std::string unicode2ansi(const std::wstring & unicode);
};

