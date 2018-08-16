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
#include "Budget.h"

using namespace std;

struct DateValue
{
	wxDateTime date;
	float value;
};

struct StringValue
{
	wxString string;
	float value;
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
	std::vector<std::shared_ptr<Account>> GetAccounts(AccountTypes type);
	std::shared_ptr<Account> GetAccountById(int id);
	std::vector<std::shared_ptr<Transaction>> GetTransactions(Account *account, wxDateTime *from, wxDateTime *to);
	std::vector<std::shared_ptr<Transaction>> GetTransactionsByType(AccountTypes type, wxDateTime *from, wxDateTime *to);
	std::vector<std::shared_ptr<Transaction>> GetDeletedTransactions();
	std::vector<std::shared_ptr<Currency>> GetCurrencies();
	std::vector<std::shared_ptr<Budget>> GetBudgets();
	std::vector<std::shared_ptr<Report>> GetReports();
	std::shared_ptr<Report> GetReportById(int id);
	float GetBalance(Account *account);
	float GetAccountTotalExpense(Account *account);
	float GetAccountTotalReceipt(Account *account);
	float GetToAmountSum(Account *account, wxDateTime *from, wxDateTime *to);
	float GetExpenses(wxDateTime *from, wxDateTime *to);
	float GetReceipts(wxDateTime *from, wxDateTime *to);
	std::vector<DateValue> GetExpensesByMonth(Account *account, wxDateTime *from, wxDateTime *to);
	std::vector<StringValue> GetExpensesByAccount(wxDateTime *from, wxDateTime *to);
	std::vector<StringValue> GetExpensesForAccount(Account *account, wxDateTime *from, wxDateTime *to);
	std::vector<StringValue> GetBalanceByMonth(Account *account, wxDateTime *from, wxDateTime *to);
	float GetExpensesSumForAccount(Account *account, wxDateTime *from, wxDateTime *to);
	int GetPairAccountId(Account *account);
	std::vector<std::shared_ptr<wxString>> GetTagsBySearch(wxString search);
	int GetDeletedTransactionsCount();

private:
	DataHelper();                 
	DataHelper(DataHelper const&);

	sqlite3 *_db;
};

#endif