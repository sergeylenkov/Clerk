#pragma once

#include <string>
#include <sqlite3.h>
#include <curl/curl.h>
#include <rapidxml.hpp>

class ExchangeRatesLoader
{
public:
	ExchangeRatesLoader(sqlite3 *db);
	void Load();

private:
	sqlite3 *db;

	void Parse(std::string data);
};
