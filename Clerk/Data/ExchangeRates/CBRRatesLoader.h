#pragma once

#include "ExchangeRatesLoader.h"
#include <curl/curl.h>
#include <rapidxml/rapidxml.hpp>

using namespace std;
using namespace rapidxml;

class CBRRatesLoader : public ExchangeRatesLoader
{
public:
	CBRRatesLoader(sqlite3 *db);
	void Load();

private:
	string url;
	void Parse(string *data);
};
