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
	char *sql = "CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY, name TEXT, note TEXT, type_id INTEGER, currency_id INTEGER, icon_id INTEGER, order_id INTEGER, active INTEGER, created_at TEXT, credit_limit NUMERIC)";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY, paid_at TEXT, from_account_id INTEGER, to_account_id INTEGER, from_account_amount NUMERIC, to_account_amount NUMERIC, deleted INTEGER, note TEXT, created_at TEXT";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY, name TEXT)";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS transactions_tags (transaction_id INTEGER, tag_id INTEGER)";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS schedulers (id INTEGER PRIMARY KEY, name TEXT, active INTEGER, type INTEGER, day INTEGER, week INTEGER, month INTEGER, from_account_id INTEGER, to_account_id INTEGER, from_account_amount INTEGER, to_account_amount INTEGER, tags TEXT, prev_date TEXT, next_date TEXT)";	

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS budgets (id INTEGER PRIMARY KEY, name TEXT, period INTEGER, date TEXT, amount FLOAT, account_ids TEXT, created_at TEXT)";
	
	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS goals (id INTEGER PRIMARY KEY, name TEXT, date TEXT, amount FLOAT, account_ids TEXT, created_at TEXT)";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS exchange_rates (id INTEGER PRIMARY KEY, from_currency_id INTEGER, to_currency_id INTEGER, rate FLOAT, count INTEGER, date TEXT)";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}

void DataHelper::InitData() {
	tags.clear();

	char *sql = "SELECT t.id, t.name, COUNT(t2.id) FROM tags t LEFT JOIN transactions_tags tt ON t.id = tt.tag_id LEFT JOIN transactions t2 ON tt.transaction_id = t2.id GROUP BY t.id ORDER BY t.name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto tag = std::make_shared<Tag>();

			tag->id = sqlite3_column_int(statement, 0);
			tag->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			tag->count = sqlite3_column_int(statement, 2);

			tags.push_back(tag);
		}
	}

	sqlite3_finalize(statement);

	ReloadExchangeRate();
	ReloadAccounts();
}

void DataHelper::ReloadAccounts() {
	accounts.clear();

	char *sql = "SELECT a.id FROM accounts a ORDER BY a.type_id, a.order_id";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto account = std::make_shared<Account>(sqlite3_column_int(statement, 0));
			accounts.push_back(account);

			accountsHash[account->id] = account;
		}
	}

	sqlite3_finalize(statement);
}

std::vector<std::shared_ptr<Account>> DataHelper::GetAccounts() {
	return accounts;
}

std::vector<std::shared_ptr<Account>> DataHelper::GetAccountsByType(AccountTypes type)
{
	std::vector<std::shared_ptr<Account>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [type](const std::shared_ptr<Account>& account) {
		if (account->isActive && account->type == type) {
			return true;
		}

		return false;
	});

	return result;
}

std::shared_ptr<Account> DataHelper::GetAccountById(int id) {
	return accountsHash[id];
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

std::vector<std::shared_ptr<Transaction>> DataHelper::GetTransactions(wxDateTime *from, wxDateTime *to)
{
	auto result = std::vector<std::shared_ptr<Transaction>>();

	char *sql = "SELECT t.id FROM transactions t, accounts fa, accounts ta \
				  WHERE t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 AND fa.id = t.from_account_id AND ta.id = t.to_account_id ORDER BY t.paid_at DESC, t.created_at DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

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

std::vector<std::shared_ptr<Transaction>> DataHelper::GetRecentTransactions() {
	auto result = std::vector<std::shared_ptr<Transaction>>();

	char *sql = "SELECT MAX(t.paid_at), t.id FROM transactions t GROUP BY t.from_account_id, t.to_account_id ORDER BY paid_at DESC LIMIT 10";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 1);
			auto transaction = std::make_shared<Transaction>(id);
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

std::vector<std::shared_ptr<Scheduler>> DataHelper::GetSchedulers(wxDateTime *from, wxDateTime *to) {
	auto result = std::vector<std::shared_ptr<Scheduler>>();

	char *sql = "SELECT id FROM schedulers WHERE next_date >= ? AND next_date <= ? AND active = 1 ORDER BY next_date, name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto scheduler = make_shared<Scheduler>(sqlite3_column_int(statement, 0));
			result.push_back(scheduler);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Goal>> DataHelper::GetGoals() {
	auto result = std::vector<std::shared_ptr<Goal>>();

	char *sql = "SELECT id FROM goals ORDER BY name";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			auto goal = make_shared<Goal>(sqlite3_column_int(statement, 0));
			result.push_back(goal);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<std::shared_ptr<Report>> DataHelper::GetReports() {
	reports.clear();

	auto report = make_shared<Report>(1);
	report->name = make_shared<wxString>("Expenses By Month");

	reports.push_back(report);

	report = make_shared<Report>(2);
	report->name = make_shared<wxString>("Balance By Month");

	reports.push_back(report);

	report = make_shared<Report>(3);
	report->name = make_shared<wxString>("Expenses For Period");

	reports.push_back(report);

	return reports;
}

std::shared_ptr<Report> DataHelper::GetReportById(int id) {
	for (auto report : reports)
	{
		if (report->id == id) {
			return report;
		}
	}

	return nullptr;
}

std::vector<std::shared_ptr<Tag>> DataHelper::GetTags() {
	return tags;
}

float DataHelper::GetBalance(Account *account)
{
	float receipt = 0.0;
	float expense = 0.0;

	char *sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	float total;

	if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Expens) {
		total = expense - receipt;
	}
	else {
		total = receipt - expense;
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

float  DataHelper::GetExpenses(Account *account, wxDateTime *from, wxDateTime *to) {
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

float DataHelper::GetReceipts(Account *account, wxDateTime *from, wxDateTime *to) {
	float total = 0;

	char *sql = "SELECT TOTAL(t.from_account_amount) FROM transactions t, accounts a WHERE a.type_id = 0 AND t.from_account_id = ? AND t.from_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
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

vector<DateValue> DataHelper::GetBalanceByMonth(Account *account, wxDateTime *from, wxDateTime *to) {
	vector<DateValue> values;

	if (account->id == -1) {
		wxDateSpan diff = to->DiffAsDateSpan(*from);

		for (int i = 0; i < diff.GetMonths(); i++) {
			wxDateTime date = from->Add(wxDateSpan(0, 1, 0, 0));
			date.SetDay(1);

			wxDateTime queryDate = wxDateTime(date);
			queryDate.SetToLastMonthDay();

			float expenses = 0.0;
			float receipt = 0.0;

			char *sql = "SELECT TOTAL(t.to_account_amount) AS amount FROM transactions t, accounts a WHERE a.type_id = 1 AND t.to_account_id = a.id AND t.deleted = 0 AND t.paid_at <= ?";
			sqlite3_stmt *statement;

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_text(statement, 1, queryDate.FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					expenses = sqlite3_column_double(statement, 0);
				}
			}

			sqlite3_finalize(statement);

			sql = "SELECT TOTAL(t.from_account_amount) AS amount FROM transactions t, accounts a WHERE a.type_id = 1 AND t.from_account_id = a.id AND t.deleted = 0 AND t.paid_at <= ?";

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_text(statement, 1, queryDate.FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					receipt = sqlite3_column_double(statement, 0);
				}
			}

			sqlite3_finalize(statement);

			float amount = expenses - receipt;

			if (account->creditLimit > 0) {
				amount = account->creditLimit + amount;
			}

			DateValue value = { date, amount };
			values.push_back(value);
		}
	}
	else {
		wxDateSpan diff = to->DiffAsDateSpan(*from);

		for (int i = 0; i < diff.GetMonths(); i++) {
			wxDateTime date = from->Add(wxDateSpan(0, 1, 0, 0));
			date.SetDay(1);

			wxDateTime queryDate = wxDateTime(date);
			queryDate.SetToLastMonthDay();

			float expenses = 0.0;
			float receipt = 0.0;

			char *sql = "SELECT TOTAL(t.to_account_amount) AS amount FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 AND t.paid_at <= ?";
			sqlite3_stmt *statement;
			
			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_int(statement, 1, account->id);
				sqlite3_bind_text(statement, 2, queryDate.FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					expenses = sqlite3_column_double(statement, 0);
				}
			}

			sqlite3_finalize(statement);

			sql = "SELECT TOTAL(t.from_account_amount) AS amount FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 AND t.paid_at <= ?";

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_int(statement, 1, account->id);
				sqlite3_bind_text(statement, 2, queryDate.FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

				if (sqlite3_step(statement) == SQLITE_ROW) {
					receipt = sqlite3_column_double(statement, 0);
				}
			}

			sqlite3_finalize(statement);
			
			float amount = expenses - receipt;

			if (account->creditLimit > 0) {
				amount = account->creditLimit + amount;
			}

			DateValue value = { date, amount };
			values.push_back(value);
		}
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

	if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Receipt || account->type == AccountTypes::Virtual) {
		sql = "SELECT t.to_account_id, COUNT(*) FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? GROUP BY t.to_account_id ORDER BY COUNT(*) DESC LIMIT 1";
	}
	else if (account->type == AccountTypes::Expens || account->type == AccountTypes::Debt) {
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
		if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Receipt || account->type == AccountTypes::Virtual) {
			sql = "SELECT t.to_account_id FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 ORDER BY t.paid_at DESC LIMIT 1";
		}
		else if (account->type == AccountTypes::Expens || account->type == AccountTypes::Debt) {
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

	auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	std::wstring searchW = search.ToStdWstring();
	f.tolower(&searchW[0], &searchW[0] + searchW.size());

	for (auto tag : tags) {
		wxString searchString = *tag->name;
		std::wstring searchStringW = searchString.ToStdWstring();
		f.tolower(&searchStringW[0], &searchStringW[0] + searchStringW.size());

		std::size_t found = searchStringW.find(searchW);

		if (found != std::string::npos) {
			result.push_back(tag->name);
		}
	}

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
	std::vector<std::shared_ptr<Account>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [](const std::shared_ptr<Account>& account) {
		return !account->isActive;
	});

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

float DataHelper::GetBalanceForGoal(Goal *goal) {
	float total = 0.0;
	float receipt_sum = 0.0;
	float expense_sum = 0.0;
	char sql[512];

	snprintf(sql, sizeof(sql), "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id IN(%s) AND deleted = 0", static_cast<const char*>(goal->accountIds->c_str()));
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {		
		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	snprintf(sql, sizeof(sql), "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id IN(%s) AND deleted = 0", static_cast<const char*>(goal->accountIds->c_str()));

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {		
		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	total = receipt_sum - expense_sum;

	return total;
}

void DataHelper::ReplaceTag(int oldId, int newId) {
	char *sql = "UPDATE transactions_tags SET tag_id = ? WHERE tag_id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, newId);
		sqlite3_bind_int(statement, 2, oldId);

		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}

std::shared_ptr<Transaction> DataHelper::GetInitialTransactionForAccount(Account *account) {
	std::shared_ptr<Transaction> transaction = nullptr;

	char *sql = "SELECT t.id, t.from_account_id, t.to_account_id, t.from_account_amount, t.to_account_amount, t.paid_at FROM transactions t WHERE t.from_account_id = ? AND t.to_account_id = -1";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account->id);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			transaction = std::make_shared<Transaction>();

			transaction->id = sqlite3_column_int(statement, 0);
			transaction->fromAmount = sqlite3_column_double(statement, 3);
			transaction->toAmount = sqlite3_column_double(statement, 4);
			transaction->fromAccountId = sqlite3_column_int(statement, 1);
			transaction->toAccountId = sqlite3_column_int(statement, 2);

			auto date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 5)));

			transaction->paidAt = date;
		}
	}

	sqlite3_finalize(statement);

	return transaction;
}

void DataHelper::EmptyTrash() {
	for (auto transaction : GetDeletedTransactions())
	{
		transaction->DeleteCompletely();
	}
}

void DataHelper::CreateAccountsImageList() {
	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 62; i++) {
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

float DataHelper::ConvertCurrency(int fromId, int toId, float amount) {
	if (fromId == toId) {
		return amount;
	}

	float rate = 1;

	if (exchangeRates[std::make_pair(fromId, toId)]) {
		rate = exchangeRates[std::make_pair(fromId, toId)];
	}
	
	return amount * rate;
}

void DataHelper::ReloadExchangeRate() {
	char *sql = "SELECT from_currency_id, to_currency_id, rate, count FROM exchange_rates";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int fromId = sqlite3_column_int(statement, 0);
			int toId = sqlite3_column_int(statement, 1);
			float rate = sqlite3_column_double(statement, 2);
			int count = sqlite3_column_int(statement, 3);

			exchangeRates[std::make_pair(fromId, toId)] = count * rate;
			exchangeRates[std::make_pair(toId, fromId)] = count / rate;
		}
	}

	sqlite3_finalize(statement);
}