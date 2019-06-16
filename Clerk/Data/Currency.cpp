#include "Currency.h"

Currency::Currency()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->shortName = make_shared<wxString>();
	this->sign = make_shared<wxString>();
}

Currency::Currency(int id) 
{
	this->id = id;
	Load();
}

void Currency::Load()
{
	char *sql = "SELECT id, name, short_name, sign FROM currencies WHERE id = ?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(_db, sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			this->id = sqlite3_column_int(statement, 0);
			this->name = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 1)));
			this->shortName = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 2)));
			this->sign = make_shared<wxString>(wxString::FromUTF8((char *)sqlite3_column_text(statement, 3)));
		}
	}

	sqlite3_finalize(statement);
}

void Currency::Save()
{

}