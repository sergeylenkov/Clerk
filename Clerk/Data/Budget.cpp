#include "Budget.h"

Budget::Budget()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->amount = 0;
	this->period = BudgetPeriods::Month;
	this->type = BudgetTypes::Limit;
	this->account = nullptr;
}

Budget::Budget(int id) : Budget()
{
	this->id = id;
	Load();
}

void Budget::Load()
{
	char *sql = "SELECT b.id, b.name, b.amount, b.period, b.type, b.account_id FROM budgets b WHERE b.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->amount = sqlite3_column_int(statement, 2);
			this->period = static_cast<BudgetPeriods>(sqlite3_column_int(statement, 3));
			this->type = static_cast<BudgetTypes>(sqlite3_column_int(statement, 4));

			int accountId = sqlite3_column_int(statement, 5);

			if (accountId != -1) {
				this->account = make_shared<Account>(accountId);
			}
		}
	}

	sqlite3_finalize(statement);
}

void Budget::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE budgets SET name = ?, amount = ?, period = ?, type = ?, account_id = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {			
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, this->amount);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->type));

			if (this->account) {
				sqlite3_bind_int(statement, 5, this->account->id);
			}
			else {
				sqlite3_bind_int(statement, 5, -1);
			}

			sqlite3_bind_int(statement, 6, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO budgets (name, amount, period, type, account_id, created_at) VALUES (?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, this->amount);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->type));

			if (this->account) {
				sqlite3_bind_int(statement, 5, this->account->id);
			}
			else {
				sqlite3_bind_int(statement, 5, -1);
			}

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