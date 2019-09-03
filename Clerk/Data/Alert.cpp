#include "Alert.h"

Alert::Alert()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->amount = 0;	
	this->accountIds = make_shared<wxString>();
	this->periodName = make_shared<wxString>("Month");
}

Alert::Alert(int id) : Alert()
{
	this->id = id;
	Load();
}

void Alert::Load()
{
	char *sql = "SELECT a.id, a.name, a.type, a.period, a.condition, a.amount, a.account_ids, a.created_at FROM alerts a WHERE a.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->type = static_cast<Types>(sqlite3_column_int(statement, 2));
			this->period = static_cast<Period>(sqlite3_column_int(statement, 3));
			this->condition = static_cast<Condition>(sqlite3_column_int(statement, 4));
			this->amount = sqlite3_column_double(statement, 5);
			this->accountIds = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 6)));

			this->createdDate = make_shared<wxDateTime>();
			this->createdDate->ParseISOCombined(wxString::FromUTF8((char *)sqlite3_column_text(statement, 7)), ' ');

			switch (period)
			{
				case Period::Week:
					periodName = make_shared<wxString>("Week");
					break;
				case Period::Month:
					periodName = make_shared<wxString>("Month");
					break;
				case Period::Year:
					periodName = make_shared<wxString>("Year");
					break;
				default:
					break;
			}
		}
	}

	sqlite3_finalize(statement);
}

void Alert::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE alerts SET name = ?, type = ?, period = ?, condition = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->condition));
			sqlite3_bind_int(statement, 5, this->amount);
			sqlite3_bind_text(statement, 6, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 7, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO alerts (name, type, period, condition, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 3, static_cast<int>(this->period));
			sqlite3_bind_int(statement, 4, static_cast<int>(this->condition));
			sqlite3_bind_int(statement, 5, this->amount);
			sqlite3_bind_text(statement, 6, this->accountIds->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 7, wxDateTime::Now().FormatISOCombined(' ').ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Alert::Delete()
{
	char *sql = "DELETE FROM alerts WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}