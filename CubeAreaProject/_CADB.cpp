#include "stdafx.h"
#include "_CADB.h"

#ifdef USE_SQL_SERVER
_CADB::_CADB()
	: pConnect(NULL)
	, pRecordset(NULL)
{
	CoInitialize(NULL);
	HRESULT hr = pConnect.CreateInstance(__uuidof(Connection));
	if (FAILED(hr))
	{
		std::cerr << "Can not create connect instance." << std::endl;
		return;
	}
	_bstr_t strConnect = "Provider=SQLOLEDB;Server=DESKTOP-ID3QIEK\\CUBEAREADB;Database=ACGNDatabase";
	try
	{
		pConnect->Open(strConnect, "sa", "SOSYUKIN", adModeUnknown);
	}
	catch (_com_error &e)
	{
		
		std::cerr << "Initiate failed!" << std::endl;
		std::cerr << e.Description() << std::endl;
		std::cerr << e.HelpFile() << std::endl;
		return;
	}
	//std::cout << "Connect succeed!" << std::endl;
	hr = pRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		std::cerr << "Can not create recordset instance." << std::endl;
		return;
	}
}


_CADB::~_CADB()
{
	try
	{
		//pRecordset->Close();
		pRecordset.Release();
		pConnect->Close();
		pConnect.Release();
	}
	catch (_com_error &e)
	{
		std::cerr << e.Description() << std::endl;
		std::cerr << e.HelpFile() << std::endl;
		return;
	}
	CoUninitialize();
	return;
}

bool _CADB::Execute(const _bstr_t & sql)
{
	try
	{
		pRecordset = pConnect->Execute(sql, NULL, adCmdText);
		//pRecordset->Open(sql, (IDispatch *)pConnect, adOpenDynamic, adLockOptimistic, adCmdText);
		/*
		std::vector<_bstr_t> column_name;
		for (size_t i = 0; i < pRecordset->Fields->GetCount(); i++)
		{
			std::cout << pRecordset->Fields->GetItem(_variant_t((long)i))->GetName() << " ";
			column_name.push_back(pRecordset->Fields->GetItem(_variant_t((long)i))->Name);
		}
		std::cout << std::endl;
		while (!pRecordset->adoEOF)
		{
			for (auto i = column_name.begin(); i != column_name.end(); i++)
			{
				if (pRecordset->GetCollect(*i).vt != VT_NULL)
				{
					std::cout << (_bstr_t)pRecordset->GetCollect((*i)) << " ";
				}
				else
				{
					std::cout << "NULL" << std::endl;
				}
			}
			pRecordset->MoveNext();
			std::cout << std::endl;
		}*/
		
	}
	catch (_com_error &e)
	{
		std::cerr << "Can not execute this sql." << std::endl;
		std::cerr << e.Description() << std::endl;
		//std::cerr << e.HelpFile() << std::endl;
		return false;
	}
	return true;
}

bool _CADB::EmptySet(const _bstr_t & sql)
{
	if (Execute(sql))
	{
		if (!pRecordset->BOF)
		{
			pRecordset->MoveFirst();
		}
		else
		{
			//std::cerr << "Data is empty!" << std::endl;
			return true;
		}
		pRecordset->Close();
	}
	return false;
}

_bstr_t _CADB::ColumnToSQL(const std::vector<_variant_t>& columnList)
{
	_bstr_t sql;
	for (auto i = columnList.begin(); i != columnList.end();)
	{
		sql += _bstr_t(L"[") + _bstr_t(*i) + _bstr_t(L"]");
		if (++i != columnList.end())
		{
			sql += _bstr_t(L",");
		}
	}
	return sql;
}

_bstr_t _CADB::ValuesToSQL(const std::vector<_variant_t>& valueList)
{
	_bstr_t sql;
	std::wstring str;
	for (auto i = valueList.begin(); i != valueList.end();)
	{
		switch (i->vt)
		{
		case VT_BSTR:
			 str = _bstr_t(*i);
			_CACodeLab::EscapeSequence(str);
			sql += _bstr_t(L"N'") + _bstr_t(str.c_str()) + _bstr_t("'");
			break;
		case VT_INT:
			sql += _bstr_t(*i);
			break;
		default:
			std::cerr << i->vt << std::endl;
		}
		if (++i != valueList.end())
		{
			sql += _bstr_t(L",");
		}
	}
	return sql;
}

bool _CADB::Insert(const _variant_t & table, const std::vector<_variant_t>& columnList, const std::vector<_variant_t>& valueList)
{
	_bstr_t sql = _bstr_t(L"INSERT INTO ") + _bstr_t(table)
		+ _bstr_t(L" (") + ColumnToSQL(columnList) + _bstr_t(L") VALUES (") + ValuesToSQL(valueList) + _bstr_t(L")");
	return Execute(sql);
}

#endif
