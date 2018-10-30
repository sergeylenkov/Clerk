#include "DataHelper.h"

DataHelper::DataHelper()
{
	
}

DataHelper::~DataHelper()
{
	sqlite3_close(_db);
	delete accountsImageList;
}

void DataHelper::Open(char *database)
{
	if (sqlite3_open(database, &_db) != SQLITE_OK) {
		sqlite3_close(_db);
	}
}

sqlite3* DataHelper::Connection() {
	return _db;
}

void DataHelper::Init() {
	char *sql = "CREATE TABLE IF NOT EXISTS schedulers (id INTEGER PRIMARY KEY, name TEXT, active INTEGER, type INTEGER, day INTEGER, week INTEGER, month INTEGER, from_account_id INTEGER, to_account_id INTEGER, from_account_amount INTEGER, to_account_amount INTEGER, tags TEXT, prev_date TEXT, next_date TEXT)";
	sqlite3_stmt *statement;	

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS budgets (id INTEGER PRIMARY KEY, name TEXT, period INTEGER, date TEXT, amount FLOAT, account_ids TEXT, created_at TEXT)";
	
	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}

std::vector<std::shared_ptr<Account>> DataHelper::GetAccounts(AccountTypes type)
{
	auto result = std::vector<std::shared_ptr<Account>>();

	char *sql = "SELECT a.id FROM accounts a WHERE a.type_id = ? AND a.active = 1 ORDER BY a.order_id";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, (int)type);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto account = std::make_shared<Account>(sqlite3_column_int(statement, 0));
			result.push_back(account);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::shared_ptr<Account> DataHelper::GetAccountById(int id) {
	char *sql = "SELECT a.id FROM accounts a WHERE a.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, (int)id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			auto account = std::make_shared<Account>(sqlite3_column_int(statement, 0));
			return account;
		}
	}

	sqlite3_finalize(statement);

	return nullptr;
}

std::vector<std::shared_ptr<Transaction>> DataHelper::GetTransactions(Account *account, wxDateTime *from, wxDateTime *to)
{
	auto result = std::vector<std::shared_ptr<Transaction>>();

	char *sql = "SELECT t.id FROM transactions t, accounts fa, accounts ta \
				  WHERE (t.from_account_id = ? OR t.to_account_id = ?) AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 AND fa.id = t.from_account_id AND ta.id = t.to_account_id ORDER BY t.paid_at DESC, t.created_at DESC";
	sqlite3_stmt *statement;
	
	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);
		sqlite3_bind_int(statement, 2, account->id);
		sqlite3_bind_text(statement, 3, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 4, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto transaction = std::make_shared<Transaction>(sqlite3_column_int(statement, 0));
			result.push_back(transaction);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Transaction>> DataHelper::GetTransactionsByType(AccountTypes type, wxDateTime *from, wxDateTime *to) {
	auto result = std::vector<std::shared_ptr<Transaction>>();

	char *sql = "SELECT t.id FROM transactions t, accounts a \
								  WHERE t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 AND a.type_id = ? AND (a.id = t.to_account_id OR a.id = t.from_account_id) ORDER BY t.paid_at DESC, t.created_at DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(statement, 3, (int)type);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto transaction = std::make_shared<Transaction>(sqlite3_column_int(statement, 0));
			result.push_back(transaction);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Transaction>> DataHelper::GetDeletedTransactions() {
	auto result = std::vector<std::shared_ptr<Transaction>>();

	char *sql = "SELECT t.id FROM transactions t WHERE t.deleted = 1 ORDER BY t.paid_at DESC, t.created_at DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto transaction = std::make_shared<Transaction>(sqlite3_column_int(statement, 0));
			result.push_back(transaction);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Currency>> DataHelper::GetCurrencies()
{
	auto result = std::vector<std::shared_ptr<Currency>>();

	char *sql = "SELECT id FROM currencies ORDER BY name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto currency = make_shared<Currency>(sqlite3_column_int(statement, 0));
			result.push_back(currency);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Budget>> DataHelper::GetBudgets() {
	auto result = std::vector<std::shared_ptr<Budget>>();

	char *sql = "SELECT id FROM budgets ORDER BY name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto budget = make_shared<Budget>(sqlite3_column_int(statement, 0));
			result.push_back(budget);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Scheduler>> DataHelper::GetSchedulers() {
	auto result = std::vector<std::shared_ptr<Scheduler>>();

	char *sql = "SELECT id FROM schedulers ORDER BY name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto scheduler = make_shared<Scheduler>(sqlite3_column_int(statement, 0));
			result.push_back(scheduler);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Report>> DataHelper::GetReports() {
	auto result = std::vector<std::shared_ptr<Report>>();

	auto report = make_shared<Report>(-1);
	report->name = make_shared<wxString>("Expenses By Month");

	result.push_back(report);

	return result;
}

std::shared_ptr<Report> DataHelper::GetReportById(int id) {
	auto report = make_shared<Report>(-1);
	report->name = make_shared<wxString>("Expenses By Month");

	return report;
}

float DataHelper::GetBalance(Account *account)
{
	float receipt_sum = 0.0;
	float expense_sum = 0.0;

	char *sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	float total;

	if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Expens) {
		total = expense_sum - receipt_sum;
	}
	else {
		total = receipt_sum - expense_sum;
	}

	return total;
}

float DataHelper::GetAccountTotalExpense(Account *account) {
	float expense = 0.0;

	char *sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	return expense;
}

float DataHelper::GetAccountTotalReceipt(Account *account) {
	float receipt = 0;

	char *sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return receipt;
}

float DataHelper::GetToAmountSum(Account *account, wxDateTime *from, wxDateTime *to)
{
	float total = 0;

	char *sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND paid_at >= ? AND paid_at <= ? AND deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);
		sqlite3_bind_text(statement, 2, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return total;
}

float DataHelper::GetExpenses(wxDateTime *from, wxDateTime *to) {
	float total = 0;

	char *sql = "SELECT TOTAL(t.to_account_amount) FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return total;
}

float DataHelper::GetReceipts(wxDateTime *from, wxDateTime *to) {
	float total = 0;

	char *sql = "SELECT TOTAL(t.from_account_amount) FROM transactions t, accounts a WHERE a.type_id = 0 AND t.from_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return total;
}

vector<DateValue> DataHelper::GetExpensesByMonth(Account *account, wxDateTime *from, wxDateTime *to) {
	vector<DateValue> values;

	if (account->id == -1) {
		char *sql = "SELECT t.paid_at AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.to_account_id = a.id AND a.type_id = 2 GROUP BY strftime('%Y %m', t.paid_at) ORDER BY date";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				wxDateTime date = wxDateTime::Now();
				date.ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)));
				date.SetDay(1);

				DateValue value = { date, static_cast<float>(sqlite3_column_double(statement, 1)) };
				values.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	} else {
		char *sql = "SELECT t.paid_at AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.to_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.to_account_id = a.id AND a.type_id = 2 GROUP BY strftime('%Y %m', t.paid_at) ORDER BY date";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, account->id);
			sqlite3_bind_text(statement, 2, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 3, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				wxDateTime date = wxDateTime::Now();
				date.ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)));
				date.SetDay(1);

				DateValue value = { date, static_cast<float>(sqlite3_column_double(statement, 1)) };
				values.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	}
	

	return values;
}

vector<StringValue> DataHelper::GetExpensesByAccount(wxDateTime *from, wxDateTime *to) {
	vector<StringValue> values;

	char *sql = "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
			values.push_back(value);
		}
	}

	sqlite3_finalize(statement);

	return values;
}

vector<StringValue> DataHelper::GetExpensesForAccount(Account *account, wxDateTime *from, wxDateTime *to) {
	vector<StringValue> values;

	if (account->id == -1) {
		char *sql = "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
				values.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "SELECT tg.name AS name, SUM(t.from_account_amount) AS sum from transactions t, transactions_tags tt, tags tg WHERE t.to_account_id = ? AND t.deleted = 0 AND tt.transaction_id = t.id AND tg.id = tt.tag_id AND t.paid_at >= ? AND t.paid_at <= ? GROUP BY tg.name ORDER BY sum DESC";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, account->id);
			sqlite3_bind_text(statement, 2, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 3, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
				values.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	}	

	return values;
}

vector<StringValue> DataHelper::GetBalanceByMonth(Account *account, wxDateTime *from, wxDateTime *to) {
	vector<StringValue> values;

	if (account->id == -1) {
		char *sql = "SELECT strftime('%Y %m', t.paid_at) AS date, TOTAL(t.to_account_amount) AS amount FROM transactions t WHERE t.deleted = 0 GROUP BY date ORDER BY t.paid_at";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			while (sqlite3_step(statement) == SQLITE_ROW) {
				StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
				values.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		vector<StringValue> receipts;
		vector<StringValue> expenses;

		char *sql = "SELECT strftime('%Y %m', t.paid_at) AS date, TOTAL(t.to_account_amount) AS amount FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 GROUP BY date ORDER BY t.paid_at";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, account->id);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
				receipts.push_back(value);
			}
		}

		sqlite3_finalize(statement);

		sql = "SELECT strftime('%Y %m', t.paid_at) AS date, TOTAL(t.from_account_amount) AS amount FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 GROUP BY date ORDER BY t.paid_at";

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, account->id);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				StringValue value = { wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) };
				expenses.push_back(value);
			}
		}

		sqlite3_finalize(statement);
	}

	return values;
}

float DataHelper::GetExpensesSumForAccount(Account *account, wxDateTime *from, wxDateTime *to) {
	float total = 0;

	char *sql = "SELECT TOTAL(t.to_account_amount) FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id = ? AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);
		sqlite3_bind_text(statement, 2, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return total;
}

int DataHelper::GetPairAccountId(Account *account) {
	int id = -1;
	char *sql = "";
	
	wxDateTime fromDate = wxDateTime::Now();

	fromDate.Add(wxDateSpan::Months(-3));
	fromDate.SetDay(1);

	if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Receipt) {
		sql = "SELECT t.to_account_id, COUNT(*) FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? GROUP BY t.to_account_id ORDER BY COUNT(*) DESC LIMIT 1";
	}
	else if (account->type == AccountTypes::Expens || account->type == AccountTypes::Debt || account->type == AccountTypes::Credit) {
		sql = "SELECT t.from_account_id, COUNT(*) FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? GROUP BY t.to_account_id ORDER BY COUNT(*) DESC LIMIT 1";
	}

	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);
		sqlite3_bind_text(statement, 2, fromDate.FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			id = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	if (id == -1) {
		if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Receipt) {
			sql = "SELECT t.to_account_id FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 ORDER BY t.paid_at DESC LIMIT 1";
		}
		else if (account->type == AccountTypes::Expens || account->type == AccountTypes::Debt || account->type == AccountTypes::Credit) {
			sql = "SELECT t.from_account_id FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 ORDER BY t.paid_at DESC LIMIT 1";
		}

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, account->id);

			if (sqlite3_step(statement) == SQLITE_ROW) {
				id = sqlite3_column_int(statement, 0);
			}
		}

		sqlite3_finalize(statement);
	}

	return id;
}

vector<shared_ptr<wxString>> DataHelper::GetTagsBySearch(wxString search) {
	auto result = vector<shared_ptr<wxString>>();

	char *sql = "SELECT DISTINCT TRIM(t.name) FROM tags t WHERE t.name LIKE '%' || ? || '%' ORDER BY t.name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, search.ToUTF8(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto tag = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)));
			result.push_back(tag);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

int DataHelper::GetDeletedTransactionsCount() {
	int result = 0;

	char *sql = "SELECT COUNT(*) FROM transactions t WHERE t.deleted = 1";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {
			result = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return result;
}

std::vector<std::shared_ptr<Account>> DataHelper::GetArchiveAccounts() {
	auto result = std::vector<std::shared_ptr<Account>>();

	char *sql = "SELECT a.id FROM accounts a WHERE a.active = 0 ORDER BY a.order_id";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto account = std::make_shared<Account>(sqlite3_column_int(statement, 0));
			result.push_back(account);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

float DataHelper::GetExpensesForBudget(Budget *budget, wxDateTime *from, wxDateTime *to) {
	float total = 0;

	char sql[512];
	sqlite3_stmt *statement;

	snprintf(sql, sizeof(sql), "SELECT TOTAL(t.to_account_amount) FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id IN(%s) AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0", static_cast<const char*>(budget->accountIds->c_str()));

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	return total;
}

void DataHelper::EmptyTrash() {
	for each (auto transaction in GetDeletedTransactions())
	{
		transaction->DeleteCompletely();
	}
}

void DataHelper::CreateAccountsImageList() {
	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}
}