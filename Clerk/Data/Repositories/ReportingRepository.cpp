#include "ReportingRepository.h"

using namespace Clerk::Data;

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesByMonth(std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char* sql = "SELECT t.paid_at AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.to_account_id = a.id AND (a.type_id = 2 OR a.type_id = 3) GROUP BY strftime('%Y %m', t.paid_at) ORDER BY date";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesByMonth(int accountId, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char* sql = "SELECT t.paid_at AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.to_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.to_account_id = a.id GROUP BY strftime('%Y %m', t.paid_at) ORDER BY date";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesByMonth(std::string& accountIds, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT t.paid_at AS date, TOTAL(t.from_account_amount) AS sum FROM transactions t, accounts a WHERE t.to_account_id IN(%s) AND t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.to_account_id = a.id GROUP BY strftime('%%Y %%m', t.paid_at) ORDER BY date", static_cast<const char*>(accountIds.c_str()));

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesByAccount(std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char* sql = "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE (a.type_id = 2 OR a.type_id = 3) AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesByAccount(std::string& accountIds, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE t.to_account_id IN(%s) AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC", static_cast<const char*>(accountIds.c_str()));


	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetExpensesForAccount(int accountId, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char* sql = "SELECT tg.name AS name, SUM(t.from_account_amount) AS sum from transactions t, transactions_tags tt, tags tg WHERE t.to_account_id = ? AND t.deleted = 0 AND tt.transaction_id = t.id AND tg.id = tt.tag_id AND t.paid_at >= ? AND t.paid_at <= ? GROUP BY tg.name ORDER BY sum DESC";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetReceiptsByMonth(std::string& accountIds, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT t.paid_at AS date, TOTAL(t.to_account_amount) AS sum FROM transactions t, accounts a WHERE t.from_account_id IN(%s) AND t.deleted = 0 AND t.paid_at >= ? AND t.paid_at <= ? AND t.from_account_id = a.id GROUP BY strftime('%%Y %%m', t.paid_at) ORDER BY date", static_cast<const char*>(accountIds.c_str()));

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

std::vector<std::pair<std::wstring, float>> ReportingRepository::GetReceiptsByAccount(std::string& accountIds, std::string& fromDate, std::string& toDate) {
	std::vector<std::pair<std::wstring, float>> values;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT a.name, TOTAL(t.to_account_amount) as sum FROM transactions t, accounts a WHERE t.from_account_id IN(%s) AND t.from_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 GROUP BY a.name ORDER BY sum DESC", static_cast<const char*>(accountIds.c_str()));

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			values.push_back({ std::wstring((wchar_t*)sqlite3_column_text16(statement, 0)), static_cast<float>(sqlite3_column_double(statement, 1)) });
		}
	}

	sqlite3_finalize(statement);

	return values;
}

float ReportingRepository::GetBalanceForDate(std::string& accountIds, std::string& date) {
	std::vector<std::pair<std::wstring, float>> values;
	
	float expenses = 0.0;
	float receipt = 0.0;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT TOTAL(t.to_account_amount) AS amount FROM transactions t WHERE t.to_account_id IN (%s) AND t.deleted = 0 AND t.paid_at <= ?", static_cast<const char*>(accountIds.c_str()));	

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, date.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expenses = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	snprintf(sql, sizeof(sql), "SELECT TOTAL(t.from_account_amount) AS amount FROM transactions t WHERE t.from_account_id IN(%s) AND t.deleted = 0 AND t.paid_at <= ?", static_cast<const char*>(accountIds.c_str()));
	

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, date.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	return expenses - receipt;
}