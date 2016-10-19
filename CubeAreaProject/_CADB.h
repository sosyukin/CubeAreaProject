#pragma once
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")
#include "_CADBTable.h"

class _CADB
{
private:
	_ConnectionPtr pConnect;
public:
	_RecordsetPtr pRecordset;

	_CADB();
	~_CADB();
	bool Execute(const _bstr_t & sql);
	bool EmptySet(const _bstr_t & sql);
	
};

