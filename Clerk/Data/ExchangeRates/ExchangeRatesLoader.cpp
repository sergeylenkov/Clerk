#include "ExchangeRatesLoader.h"

ExchangeRatesLoader::ExchangeRatesLoader(sqlite3 *db) {
	_db = db;
}

void ExchangeRatesLoader::Load() {
	
}

void ExchangeRatesLoader::UpdateValue(wxDateTime *date, string *from, string *to, float value, int count) {	
	int fromId = -1;
	int toId = -1;

	char *sql = "SELECT id FROM currencies WHERE short_name = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, from->c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			fromId = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, to->c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			toId = sqlite3_column_int(statement, 0);
		}
	}

	if (fromId != -1 && toId != -1) {
		bool newRate = false;

		sql = "SELECT rate FROM exchange_rates WHERE from_currency_id = ? AND to_currency_id = ? AND date = ?";

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, fromId);
			sqlite3_bind_int(statement, 2, toId);
			sqlite3_bind_text(statement, 3, date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) != SQLITE_ROW) {
				newRate = true;
			}
		}

		sqlite3_reset(statement);

		if (newRate) {
			sql = "INSERT INTO exchange_rates (from_currency_id, to_currency_id, rate, count, date) VALUES (?, ?, ?, ?, ?)";

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_int(statement, 1, fromId);
				sqlite3_bind_int(statement, 2, toId);
				sqlite3_bind_double(statement, 3, value);
				sqlite3_bind_int(statement, 4, count);
				sqlite3_bind_text(statement, 5, date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);

				sqlite3_step(statement);
			}

			sqlite3_finalize(statement);
		}
	}
}