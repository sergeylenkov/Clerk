#ifndef Budget_h
#define Budget_h

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Currency.h"

using namespace std;

enum class BudgetTypes {
	Receipt = 0,
	Deposit = 1,
	Expens = 2
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
	shared_ptr<wxString> note;
	BudgetTypes type;
	BudgetPeriods period;
	int orderId;
	shared_ptr<Currency> currency;

	Budget();
	Budget(int id);
	void Load();
	void Save();
	void Delete();
};

#endif