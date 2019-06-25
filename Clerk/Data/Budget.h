#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Budget : public Model
{
public:
	enum class Period {
		Week = 0,
		Month = 1,
		Year = 2,
		Custom = 3
	};

	int id;
	shared_ptr<wxString> name;
	float amount;
	float balance;
	Period period;
	shared_ptr<wxString> periodName;
	shared_ptr<wxDateTime> date;
	shared_ptr<wxString> accountIds;	

	Budget();
	Budget(int id);
	void Load();
	void Save();
	void Delete();
};