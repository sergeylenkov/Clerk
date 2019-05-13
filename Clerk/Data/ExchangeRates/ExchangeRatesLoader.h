#pragma once

#include <wx/wx.h>
#include <string>
#include <sqlite3.h>

using namespace std;

class ExchangeRatesLoader
{
public:
	ExchangeRatesLoader(sqlite3 *db);
	void Load();

private:

	
protected:
	sqlite3 *db;

	void UpdateValue(wxDateTime *date, string *from, string *to, float value, int count);
};
