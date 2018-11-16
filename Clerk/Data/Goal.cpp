#include "Goal.h"

Goal::Goal()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->amount = 0;
	this->date = make_shared<wxDateTime>(wxDateTime::Now());
	this->accountIds = make_shared<wxString>();
}

Goal::Goal(int id) : Goal()
{
	this->id = id;
	Load();
}

void Goal::Load()
{
	char *sql = "SELECT g.id, g.name, g.date, g.amount, g.account_ids FROM goals g WHERE g.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));		

			auto date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 2)));

			this->date = date;

			this->amount = sqlite3_column_double(statement, 3);
			this->accountIds = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 4)));
		}
	}

	sqlite3_finalize(statement);
}

void Goal::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE goals SET name = ?, date = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);			
			sqlite3_bind_text(statement, 2, this->date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, this->amount);
			sqlite3_bind_text(statement, 4, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 5, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO goals (name, date, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);			
			sqlite3_bind_text(statement, 2, this->date->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, this->amount);
			sqlite3_bind_text(statement, 4, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 5, wxDateTime::Now().FormatISOCombined(' ').ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Goal::Delete()
{
	char *sql = "DELETE FROM goals WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}