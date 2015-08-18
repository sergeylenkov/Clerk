#include "Budget.h"

Budget::Budget()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->note = make_shared<wxString>();
	this->period = BudgetPeriods::Month;
	this->type = BudgetTypes::Deposit;
	this->orderId = 0;
	this->currency = make_shared<Currency>(152);
}

Budget::Budget(int id) : Budget()
{
	this->id = id;
	Load();
}

void Budget::Load()
{
	char *sql = "SELECT b.id, b.name, b.note, b.period, b.type, b.order, b.currency_id FROM budgets b WHERE b.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->note = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 2)));
			this->period = static_cast<BudgetPeriods>(sqlite3_column_int(statement, 3));
			this->type = static_cast<BudgetTypes>(sqlite3_column_int(statement, 4));
			this->orderId = sqlite3_column_int(statement, 5);
			this->currency = make_shared<Currency>(sqlite3_column_int(statement, 6));
		}
	}

	sqlite3_finalize(statement);
}

void Budget::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE budgets SET name = ?, note = ?, period = ?, type = ?, order = ?, currency_id = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 5, this->orderId);
			sqlite3_bind_int(statement, 6, this->currency->id);
			sqlite3_bind_int(statement, 7, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO budgets (name, note, period, type, order, currency_id, active, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 5, this->orderId);
			sqlite3_bind_int(statement, 6, this->currency->id);
			sqlite3_bind_int(statement, 7, true);
			sqlite3_bind_text(statement, 8, wxDateTime::Now().FormatISOCombined(' ').ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Budget::Delete()
{
	char *sql = "UPDATE accounts SET active = ? WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, false);
		sqlite3_bind_int(statement, 2, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}