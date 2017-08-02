#pragma once

#ifdef USE_SQL_SERVER
class _CADBRecord
{
private:

	std::vector<_variant_t> _dataList;
public:

	_CADBRecord();
	~_CADBRecord();
};

#endif