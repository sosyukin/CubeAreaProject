#pragma once
#include "_CAObject.h"

#ifdef USE_SQL_SERVER
#include "_CADBRecord.h"
class _CADBTable :
	public _CAObject
{
	enum DBFieldType
	{

	};
	struct DBField
	{
		_bstr_t name;
		DBFieldType type;
	};

	_bstr_t _tableName;
	std::vector<DBField> _fieldList;
	std::vector<_CADBRecord> _recordList;
	
public:
	_CADBTable();
	~_CADBTable();
};

#endif