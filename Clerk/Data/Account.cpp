#include "Account.h"

Account::Account()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->note = make_shared<wxString>();
	this->type = AccountType::Deposit;
	this->iconId = 0;
	this->orderId = 1000;
	this->currency = make_shared<Currency>(152);
	this->isCredit = false;
	this->creditLimit = 0;
	this->isActive = true;
	this->balance = 0;
}

Account::Account(int id) : Account()
{
	this->id = id;
	Load();
}

void Account::Load()
{
	char *sql = "SELECT a.id, a.name, a.note, a.icon_id, a.type_id, a.order_id, a.currency_id, a.credit_limit, a.active FROM accounts a WHERE a.id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->note = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 2)));
			this->iconId = sqlite3_column_int(statement, 3);
			this->type = static_cast<AccountType>(sqlite3_column_int(statement, 4));
			this->orderId = sqlite3_column_int(statement, 5);
			this->currency = make_shared<Currency>(sqlite3_column_int(statement, 6));
			this->creditLimit = sqlite3_column_double(statement, 7);
			this->isActive = sqlite3_column_int(statement, 8);

			if (this->creditLimit > 0) {
				this->isCredit = true;
			}
		}
	}
	
	sqlite3_finalize(statement);
}

void Account::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE accounts SET name = ?, note = ?, type_id = ?, icon_id = ?, order_id = ?, currency_id = ? WHERE id = ?";
		sqlite3_stmt *statement;
		
		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 4, this->iconId);
			sqlite3_bind_int(statement, 5, this->orderId);
			sqlite3_bind_int(statement, 6, this->currency->id);
			sqlite3_bind_int(statement, 7, this->id);

			int res = sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	} else {
		char *sql = "INSERT INTO accounts (name, note, type_id, icon_id, order_id, currency_id, active, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(this->type));
			sqlite3_bind_int(statement, 4, this->iconId);
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

void Account::Delete()
{
	char *sql = "UPDATE accounts SET active = ? WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, false);
		sqlite3_bind_int(statement, 2, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	isActive = false;
}

void Account::Restore() {
	char *sql = "UPDATE accounts SET active = ? WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, true);
		sqlite3_bind_int(statement, 2, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	isActive = true;
}