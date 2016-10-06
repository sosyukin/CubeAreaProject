#include "stdafx.h"
#include "_CADB.h"


_CADB::_CADB()
	: pConnect(NULL)
	, pRecordset(NULL)
{
	CoInitialize(NULL);
	HRESULT hr = pConnect.CreateInstance(__uuidof(Connection));
	if (FAILED(hr))
	{
		std::cout << "Can not create connect instance." << std::endl;
		return;
	}
	_bstr_t strConnect = "Provider=SQLOLEDB;Server=DESKTOP-EBP9289\\CUBEAREADB;Database=test";
	try
	{
		pConnect->Open(strConnect, "sa", "123456", adModeUnknown);
	}
	catch (_com_error &e)
	{
		std::cout << "Initiate failed!" << std::endl;
		std::cout << e.Description() << std::endl;
		std::cout << e.HelpFile() << std::endl;
		return;
	}
	std::cout << "Connect succeed!" << std::endl;
	hr = pRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		std::cout << "Can not create recordset instance." << std::endl;
		return;
	}
}


_CADB::~_CADB()
{
	try
	{
		pRecordset->Close();
		pConnect->Close();
		pRecordset.Release();
		pConnect.Release();
	}
	catch (_com_error &e)
	{
		std::cout << e.Description() << std::endl;
		std::cout << e.HelpFile() << std::endl;
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
		if (!pRecordset->BOF)
		{
			pRecordset->MoveFirst();
		}
		else
		{
			std::cout << "Data is empty!" << std::endl;
			return false;
		}
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
		}
	}
	catch (_com_error &e)
	{
		std::cout << "can not execute this sql." << std::endl;
		std::cout << e.Description() << std::endl;
		std::cout << e.HelpFile() << std::endl;
		return false;
	}
	return true;
}
