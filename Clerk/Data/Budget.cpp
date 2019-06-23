#include "Budget.h"

Budget::Budget()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->amount = 0;
	this->balance = 0;
	this->period = BudgetPeriods::Month;
	this->date = make_shared<wxDateTime>(wxDateTime::Now());
	this->accountIds = make_shared<wxString>();
}

Budget::Budget(int id) : Budget()
{
	this->id = id;
	Load();
}

void Budget::Load()
{
	char *sql = "SELECT b.id, b.name, b.period, b.date, b.amount, b.account_ids FROM budgets b WHERE b.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->period = static_cast<BudgetPeriods>(sqlite3_column_int(statement, 2));

			auto date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 3)));

			this->date = date;

			this->amount = sqlite3_column_double(statement, 4);
			this->accountIds = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 5)));
		}
	}

	sqlite3_finalize(statement);
}

void Budget::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE budgets SET name = ?, period = ?, date = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {			
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(this->period));
			sqlite3_bind_text(statement, 3, this->date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 4, this->amount);			
			sqlite3_bind_text(statement, 5, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 6, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO budgets (name, period, date, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);			
			sqlite3_bind_int(statement, 2, static_cast<int>(this->period));
			sqlite3_bind_text(statement, 3, this->date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 4, this->amount);
			sqlite3_bind_text(statement, 5, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 6, wxDateTime::Now().FormatISOCombined(' ').ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Budget::Delete()
{
	char *sql = "DELETE FROM budgets WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}