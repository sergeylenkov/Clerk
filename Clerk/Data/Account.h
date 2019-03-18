#pragma once

#include <wx/wx.h>
#include <memory>
#include <codecvt>
#include "Model.h"
#include "Currency.h"

using namespace std;

enum class AccountTypes {
	Receipt = 0,
	Deposit = 1,
	Expens = 2,
	Debt = 3,
	Credit = 4,
	Virtual = 5,
};

class Account : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> note;
	AccountTypes type;
	int iconId;
	int orderId;
	shared_ptr<Currency> currency;
	float creditLimit;
	boolean isActive;

	Account();
	Account(int id);
	void Load();
	void Save();
	void Delete();
	void Restore();
};