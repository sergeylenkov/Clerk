#ifndef Account_h
#define Account_h

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Currency.h"

using namespace std;

enum class AccountTypes {
	Receipt = 0,
	Deposit = 1,
	Expens = 2,
	Debt = 3,
	Credit = 4,
};

class Account : public Model
{
public:
	int id;
	shared_ptr<string> name;
	shared_ptr<string> note;
	AccountTypes type;
	int iconId;
	int orderId;
	shared_ptr<Currency> currency;
	float creditLimit;

	Account();
	Account(int id);
	void Load();
	void Save();
	void Delete();
};

#endif
