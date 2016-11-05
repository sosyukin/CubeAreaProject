#pragma once
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")
#include "_CADBTable.h"

class _CADB
{
private:
	_ConnectionPtr pConnect;
public:
	/*enum columnType
	{
		DBnchar,
		DBint
	};
	struct column
	{
		_variant_t name;
		columnType type;
	};*/
	_RecordsetPtr pRecordset;
	_CADB();
	~_CADB();
	bool Execute(const _bstr_t & sql);
	bool EmptySet(const _bstr_t & sql);
	_bstr_t ColumnToSQL(const std::vector<_variant_t> & columnList);
	_bstr_t ValuesToSQL(const std::vector<_variant_t> & valueList);
	bool Insert(const _variant_t & table, const std::vector<_variant_t> & columnList, const std::vector<_variant_t> & valueList);
	bool Update(const _variant_t & table, const std::vector<_variant_t> & columnList, const std::vector<_variant_t> & valueList, const _variant_t & constraint);
	bool Delete(const _variant_t & table, const _variant_t & constraint);
	bool Select(const std::vector<_variant_t> & tableList, const std::vector<_variant_t> & columnList, const _variant_t & constraint);
};

