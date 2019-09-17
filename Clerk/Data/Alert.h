#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Alert : public Model
{
public:
	enum class Type {
		Receipt = 0,
		Expense = 1,
		Balance = 2		
	};

	enum class Period {
		Week = 0,
		Month = 1,
		Year = 2,
	};

	enum class Condition {
		Less = 0,
		More = 1,
		Equal = 2
	};

	int id;
	shared_ptr<wxString> name;
	Type type;
	Period period;
	Condition condition;
	float amount;	
	shared_ptr<wxDateTime> createdDate;
	shared_ptr<wxString> accountIds;
	shared_ptr<wxString> typeName;
	shared_ptr<wxString> periodName;
	shared_ptr<wxString> conditionName;

	Alert();
	Alert(int id);
	void Load();
	void Save();
	void Delete();
};

