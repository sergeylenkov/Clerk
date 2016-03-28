#ifndef DataHelper_h
#define DataHelper_h

#include <wx/wx.h>
#include <wx/datetime.h>
#include <sqlite3.h>
#include <vector>
#include <memory>
#include <map>
#include "Account.h"
#include "Currency.h"
#include "Transaction.h"
#include "Report.h"

using namespace std;

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
	vector<shared_ptr<Transaction>> GetTransactionsByType(AccountTypes type, wxDateTime *from, wxDateTime *to);
	vector<shared_ptr<Currency>> GetCurrencies();
	float GetBalance(Account *account);
	float GetToAmountSum(Account *account, wxDateTime *from, wxDateTime *to);
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