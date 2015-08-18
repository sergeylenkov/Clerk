#include "Transaction.h"

Transaction::Transaction()
{
	this->id = -1;
	this->fromAccountId = -1;
	this->toAccountId = -1;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->note = make_shared<wxString>();
	this->tags = make_shared<wxString>();
	this->paidAt = make_shared<wxDateTime>(wxDateTime::Now());
}

Transaction::Transaction(int id) : Transaction()
{
	this->id = id;
	Load();
}

void Transaction::Load()
{
	char *sql = "SELECT t.id, t.from_account_amount, t.to_account_amount, t.paid_at, fa.name, ta.name, fa.id, ta.id, t.note \
								   FROM transactions t, accounts fa, accounts ta WHERE t.id = ? AND fa.id = t.from_account_id AND ta.id = t.to_account_id";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->fromAmount = sqlite3_column_double(statement, 1);
			this->toAmount = sqlite3_column_double(statement, 2);

			auto date = make_shared<wxDateTime>();
			date->ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 3)));

			this->paidAt = date;

			this->fromAccountName = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 4)));
			this->toAccountName = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 5)));

			this->fromAccountId = sqlite3_column_int(statement, 6);
			this->toAccountId = sqlite3_column_int(statement, 7);
			this->note = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 8)));

			wxString tags;

			char *sql = "SELECT t.id, t.name FROM tags t, transactions_tags tt WHERE tt.transaction_id = ? AND t.id = tt.tag_id";
			sqlite3_stmt *statement;

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_int(statement, 1, this->id);

				while (sqlite3_step(statement) == SQLITE_ROW) {
					tags += wxString::FromUTF8((char *)sqlite3_column_text(statement, 1));
					tags += ", ";
				}

				tags.RemoveLast(2);

				this->tags = make_shared<wxString>(tags);
			}
		}
	}

	sqlite3_finalize(statement);
}

void Transaction::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE transactions SET from_account_id = ?, to_account_id = ?, from_account_amount = ?, to_account_amount = ?, paid_at = ?, note = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 7, this->id);
			sqlite3_bind_int(statement, 1, this->fromAccountId);
			sqlite3_bind_int(statement, 2, this->toAccountId);
			sqlite3_bind_double(statement, 3, this->fromAmount);
			sqlite3_bind_double(statement, 4, this->toAmount);
			sqlite3_bind_text(statement, 5, this->paidAt->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 6, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);

		UpdateTags();
	} else {
		char *sql = "INSERT INTO transactions (from_account_id, to_account_id, from_account_amount, to_account_amount, deleted, paid_at, note, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, this->fromAccountId);
			sqlite3_bind_int(statement, 2, this->toAccountId);
			sqlite3_bind_double(statement, 3, this->fromAmount);
			sqlite3_bind_double(statement, 4, this->toAmount);
			sqlite3_bind_int(statement, 5, false);
			sqlite3_bind_text(statement, 6, this->paidAt->FormatISODate().ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 7, this->note->ToUTF8(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 8, wxDateTime::Now().FormatISOCombined(' ').ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);

		if (this->id != -1) {
			UpdateTags();
		}
	}
}

void Transaction::Delete()
{
	char *sql = "DELETE FROM transactions WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "DELETE FROM transactions_tags WHERE transaction_id = ?";

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}

void Transaction::UpdateTags()
{
	char *sql = "DELETE FROM transactions_tags WHERE transaction_id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	wxStringTokenizer tokenizer(*this->tags, ",");

	while (tokenizer.HasMoreTokens())
	{
		wxString token = tokenizer.GetNextToken().Trim();
		int tag_id = -1;

		sql = "SELECT id FROM tags WHERE name = ?";

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, token.ToUTF8(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_ROW) {
				tag_id = sqlite3_column_int(statement, 0);
			}
		}

		if (tag_id == -1) {
			sql = "INSERT INTO tags (name) VALUES (?)";

			if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
				sqlite3_bind_text(statement, 1, token.ToUTF8(), -1, SQLITE_TRANSIENT);

				if (sqlite3_step(statement) == SQLITE_DONE) {
					tag_id = sqlite3_last_insert_rowid(_db);
				}
			}

			sqlite3_finalize(statement);
		}

		sql = "INSERT INTO transactions_tags (tag_id, transaction_id) VALUES (?, ?)";

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, tag_id);
			sqlite3_bind_int(statement, 2, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
}