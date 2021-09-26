#include "DataConnection.h"

using namespace Clerk::Data;

DataConnection::DataConnection(std::string&& path) {
	_path = path;
	Open();
}

DataConnection::~DataConnection() {
	Close();
}

void DataConnection::Open() {
	if (sqlite3_open_v2(_path.c_str(), &_connection, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
		Close();
	}
}

void DataConnection::Close() {
	if (_connection) {
		sqlite3_close(_connection);
		_connection = nullptr;
	}
}

sqlite3* DataConnection::GetConnection() {
	return _connection;
}

void DataConnection::CreateDatabase() {
	char* sql = "CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY, name TEXT, note TEXT, type_id INTEGER, currency_id INTEGER, icon_id INTEGER, order_id INTEGER, active INTEGER, created_at TEXT, credit_limit REAL)";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY, paid_at TEXT, from_account_id INTEGER, to_account_id INTEGER, from_account_amount REAL, to_account_amount REAL, deleted INTEGER, note TEXT, created_at TEXT";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY, name TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS transactions_tags (transaction_id INTEGER, tag_id INTEGER)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS schedulers (id INTEGER PRIMARY KEY, name TEXT, active INTEGER, type INTEGER, day INTEGER, week INTEGER, month INTEGER, from_account_id INTEGER, to_account_id INTEGER, from_account_amount INTEGER, to_account_amount INTEGER, tags TEXT, prev_date TEXT, next_date TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS budgets (id INTEGER PRIMARY KEY, name TEXT, period INTEGER, date TEXT, amount FLOAT, account_ids TEXT, created_at TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS goals (id INTEGER PRIMARY KEY, name TEXT, date TEXT, amount FLOAT, account_ids TEXT, created_at TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS exchange_rates (id INTEGER PRIMARY KEY, from_currency_id INTEGER, to_currency_id INTEGER, rate REAL, count INTEGER, date TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "CREATE TABLE IF NOT EXISTS alerts (id INTEGER PRIMARY KEY, name TEXT, type INTEGER, period INTEGER, condition INTEGER, amount REAL, account_ids TEXT, created_at TEXT)";

	if (sqlite3_prepare_v2(_connection, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}