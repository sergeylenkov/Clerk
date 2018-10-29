#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Account.h"

using namespace std;

enum class BudgetPeriods {
	Week = 0,
	Month = 1,
	Year = 2,
	Custom = 3
};

class Budget : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	float amount;
	BudgetPeriods period;
	shared_ptr<wxDateTime> date;
	shared_ptr<wxString> accountIds;

	Budget();
	Budget(int id);
	void Load();
	void Save();
	void Delete();
};