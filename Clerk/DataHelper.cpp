#include "DataHelper.h"

DataHelper::DataHelper()
{

}

DataHelper::~DataHelper()
{
	sqlite3_close(_db);
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

vector<std::shared_ptr<Account>> DataHelper::GetAccounts(AccountTypes type)
{
	auto result = vector<shared_ptr<Account>>();

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

vector<std::shared_ptr<Transaction>> DataHelper::GetTransactions(Account *account, wxDateTime *from, wxDateTime *to)
{
	auto result = vector<std::shared_ptr<Transaction>>();

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

vector<std::shared_ptr<Currency>> DataHelper::GetCurrencies() 
{
	auto result = vector<std::shared_ptr<Currency>>();

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

map<wxString, float> DataHelper::GetExpensesByMonth() {
	map<wxString, float> values;

	char *sql = "SELECT strftime('%Y %m', t.paid_at) AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.deleted = 0 AND t.to_account_id = a.id AND a.type_id = 2 GROUP BY strftime('%Y %m', t.paid_at) ORDER BY date";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			values[wxString::FromUTF8((char *)sqlite3_column_text(statement, 0))] = sqlite3_column_double(statement, 1);
		}
	}

	sqlite3_finalize(statement);

	return values;
}

map<wxString, float> DataHelper::GetExpensesByAccount(wxDateTime *from, wxDateTime *to) {
	map<wxString, float> values;

	char *sql = "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE a.type_id = 2 AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, to->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values[wxString::FromUTF8((char *)sqlite3_column_text(statement, 0))] = sqlite3_column_double(statement, 1);
		}
	}

	sqlite3_finalize(statement);

	return values;
}