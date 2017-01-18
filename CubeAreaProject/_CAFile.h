#pragma once
#include "stdafx.h"
class _CAFile : 
	public _CAFileBase
{
public:
	enum ATTR
	{
		FILENAME,
		SUFFIX
	};
	_CAFile();
	_CAFile(const std::wstring & filePath);
	~_CAFile();

	bool Open(const std::wstring & filePath);
	std::wstring Suffix() { return _suffix; }
	__int64 Size() { return _size; }
	__int64 Offset() { return _offset; }
	virtual bool IsFolder();
	bool Eof();
	bool Diff();
	size_t Read(char * buffer, const size_t & offsetBegin, const size_t & expectedReadLength);
	virtual bool Rename(const std::wstring & newName);
	bool Constraint(const std::wstring & constraintStr, bool(*ConstraintFunction)(const std::wstring &, const std::wstring &), _CAFile::ATTR attr);
	// beta
	bool md5();
	bool sha1();
private:
	std::wstring _suffix;
	std::wstring _sha1Report;
	size_t _size;
	size_t _offset;
};

