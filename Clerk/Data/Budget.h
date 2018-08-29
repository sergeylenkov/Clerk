#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Account.h"

using namespace std;

enum class BudgetTypes {
	Limit = 0,
	Goal = 1,
};

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
	BudgetTypes type;
	BudgetPeriods period;
	shared_ptr<Account> account;

	Budget();
	Budget(int id);
	void Load();
	void Save();
	void Delete();
};