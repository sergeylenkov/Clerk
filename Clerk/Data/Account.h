#pragma once

#include <wx/wx.h>
#include <memory>
#include <codecvt>
#include "Model.h"
#include "Currency.h"

using namespace std;

enum class AccountType {
	Receipt = 0,
	Deposit = 1,
	Expens = 2,
	Debt = 3,
	Virtual = 4,
};

class Account : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> note;
	AccountType type;
	int iconId;
	int orderId;
	shared_ptr<Currency> currency;
	boolean isCredit;
	float creditLimit;
	boolean isActive;
	float balance;

	Account();
	Account(int id);
	void Load();
	void Save();
	void Delete();
	void Restore();
};