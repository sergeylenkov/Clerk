#include "Tag.h"

Tag::Tag()
{
	this->id = -1;
	this->name = make_shared<wxString>();
}

Tag::Tag(int id)
{
	this->id = id;
	Load();
}

void Tag::Load()
{
	char *sql = "SELECT id, name FROM tags WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));			
		}
	}

	sqlite3_finalize(statement);
}

void Tag::Save()
{
	if (this->id != -1) {
		char *sql = "UPDATE tags SET name = ? WHERE id = ?";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);			
			sqlite3_bind_int(statement, 2, this->id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
	else {
		char *sql = "INSERT INTO tags (name) VALUES (?)";
		sqlite3_stmt *statement;

		if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, this->name->ToUTF8(), -1, SQLITE_TRANSIENT);			

			if (sqlite3_step(statement) == SQLITE_DONE) {
				this->id = sqlite3_last_insert_rowid(_db);
			}
		}

		sqlite3_finalize(statement);
	}
}

void Tag::Delete()
{
	char *sql = "DELETE FROM tags WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, this->id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}