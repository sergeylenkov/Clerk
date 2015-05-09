#ifndef DataHelper_h
#define DataHelper_h

#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>
#include <sqlite3.h>
#include <vector>
#include <memory>
#include <map>

using namespace std;

enum class AccountTypes {
	Receipt = 0,
	Deposit = 1,
	Expens = 2,
	Debt = 3
};

class Currency 
{
public:
	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> shortName;

	Currency::~Currency() {
	}
};

class Account
{
public:
	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> note;
	AccountTypes type;
	int iconId;
	int orderId;
	shared_ptr<Currency> currency;

	Account::~Account() {
	}
};

class Transaction
{
public:
	int id;
	int from_account_id;
	int to_account_id;
	shared_ptr<wxString> to_account_name;
	shared_ptr<wxString> from_account_name;
	float from_amount;
	float to_amount;
	shared_ptr<wxDateTime> paid_at;
	shared_ptr<wxString> note;
	shared_ptr<wxString> tags;

	Transaction::~Transaction() {
	}
};

class DataHelper
{
public:
	~DataHelper();

	static DataHelper& GetInstance() {
		static DataHelper instance;
		return instance;
	}

	void Open(char *database);
	sqlite3* Connection();
	vector<shared_ptr<Account>> GetAccounts(AccountTypes type);
	vector<shared_ptr<Transaction>> GetTransactions(Account *account, wxDateTime *from, wxDateTime *to);
	vector<shared_ptr<Currency>> GetCurrencies();
	float GetBalance(Account *account);
	float GetToAmountSum(Account *account, wxDateTime *from, wxDateTime *to);
	Transaction *GetTransaction(int id);
	void AddTransaction(Transaction *transaction);
	void UpdateTransaction(Transaction *transaction);
	void DeleteTransaction(int id);
	void UpdateTags(Transaction *transaction);
	shared_ptr<Currency> GetCurrency(int id);
	void AddAccount(Account *account);
	void UpdateAccount(Account *account);
	void DeleteAccount(int id);
	float GetExpenses(wxDateTime *from, wxDateTime *to);
	float GetReceipts(wxDateTime *from, wxDateTime *to);
	map<wxString, float> GetExpensesByMonth();
	map<wxString, float> GetExpensesByAccount(wxDateTime *from, wxDateTime *to);

private:
	DataHelper();                 
	DataHelper(DataHelper const&);
	void operator=(DataHelper const&);

	sqlite3 *_db;
};

#endif