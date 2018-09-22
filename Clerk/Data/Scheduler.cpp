#include "Scheduler.h"

Scheduler::Scheduler()
{
	this->id = -1;
	this->name = make_shared<wxString>();	
	this->type = SchedulerTypes::Daily;
	this->day = 1;
	this->week = 1;
	this->month = 1;
	this->fromAccount = nullptr;
	this->toAccount = nullptr;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->tags = make_shared<wxString>();
	this->previousDate = make_shared<wxDateTime>(wxDateTime::Now());
	this->nextDate = make_shared<wxDateTime>(wxDateTime::Now());
	this->active = false;
}

Scheduler::Scheduler(int id) : Scheduler()
{
	this->id = id;
	Load();
}

void Scheduler::Load()
{
	char *sql = "SELECT s.id, s.name, s.type, s.day, s.week, s.month, s.from_account_id, s.to_account_id, s.from_account_amount, s.to_account_amount, s.tags, s.prev_date, s.next_date, s.active FROM schedulers s WHERE s.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->type = static_cast<SchedulerTypes>(sqlite3_column_int(statement, 2));
			this->day = sqlite3_column_int(statement, 3);
			this->week = sqlite3_column_int(statement, 4);
			this->month = sqlite3_column_int(statement, 5);
			this->fromAmount = sqlite3_column_double(statement, 8);
			this->toAmount = sqlite3_column_double(statement, 9);			
			this->tags = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 10)));
			this->active = sqlite3_column_int(statement, 13);

			auto date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 11)));

			this->previousDate = date;

			date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 12)));

			this->nextDate = date;

			int accountId = sqlite3_column_int(statement, 6);

			if (accountId != -1) {
				this->fromAccount = make_shared<Account>(accountId);
			}

			accountId = sqlite3_column_int(statement, 7);

			if (accountId != -1) {
				this->toAccount = make_shared<Account>(accountId);
			}
		}
	}

	sqlite3_finalize(statement);
}

void Scheduler::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE schedulers SET name = ?, type = ?, day = ?, week = ?, month = ?, from_account_id = ?, to_account_id = ?, from_account_amount = ?, to_account_amount = ?, tags = ?, prev_date = ?, next_date = ?, active = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 3, this->day);
			sqlite3_bind_int(statement, 4, this->week);
			sqlite3_bind_int(statement, 5, this->month);
			sqlite3_bind_int(statement, 6, this->fromAccount->id);
			sqlite3_bind_int(statement, 7, this->toAccount->id);
			sqlite3_bind_double(statement, 8, this->fromAmount);
			sqlite3_bind_double(statement, 9, this->toAmount);
			sqlite3_bind_text(statement, 10, this->tags->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 11, this->previousDate->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 12, this->nextDate->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 13, this->active);

			sqlite3_bind_int(statement, 14, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO schedulers (name, type, day, week, month, from_account_id, to_account_id, from_account_amount, to_account_amount, tags, prev_date, next_date, active) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 3, this->day);
			sqlite3_bind_int(statement, 4, this->week);
			sqlite3_bind_int(statement, 5, this->month);
			sqlite3_bind_int(statement, 6, this->fromAccount->id);
			sqlite3_bind_int(statement, 7, this->toAccount->id);
			sqlite3_bind_double(statement, 8, this->fromAmount);
			sqlite3_bind_double(statement, 9, this->toAmount);
			sqlite3_bind_text(statement, 10, this->tags->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 11, this->previousDate->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 12, this->nextDate->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 13, this->active);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Scheduler::Delete()
{
	char *sql = "DELETE FROM schedulers WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}