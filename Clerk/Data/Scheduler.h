#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Account.h"
#include "Transaction.h"

using namespace std;

class Scheduler : public Model
{
public:
	enum class Type {
		Daily = 0,
		Weekly = 1,
		Monthly = 2,
		Yearly = 3
	};

	int id;
	shared_ptr<wxString> name;
	Type type;
	shared_ptr<wxString> typeName;
	int day;
	int week;
	int month;
	shared_ptr<Account> fromAccount;
	shared_ptr<Account> toAccount;
	float fromAmount;
	float toAmount;	
	shared_ptr<wxString> tags;
	shared_ptr<wxDateTime> previousDate;
	shared_ptr<wxDateTime> nextDate;	
	boolean active;

	Scheduler();
	Scheduler(int id);
	void Load();
	void Save();
	void Delete();
	void Run();
	void Pause();
	void CalculateNextDate();
	void Execute();
};

