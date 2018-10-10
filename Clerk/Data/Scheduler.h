#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Account.h"
#include "Transaction.h"

using namespace std;

enum class SchedulerTypes {
	Daily = 0,
	Weekly = 1,
	Monthly = 2,
	Yearly = 3
};

class Scheduler : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	SchedulerTypes type;
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

