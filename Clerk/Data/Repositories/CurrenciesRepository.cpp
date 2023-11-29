#include "CurrenciesRepository.h"

using namespace Clerk::Data;

CurrencyModel* CurrenciesRepository::GetById(int id) {
	return Load(id);
}

std::vector<CurrencyModel*> CurrenciesRepository::GetAll() {
	std::vector<CurrencyModel*> result;

	char* sql = "SELECT id FROM currencies ORDER BY name";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);
			result.push_back(Load(id));
		}
	}

	sqlite3_finalize(statement);

	return result;
}

CurrencyModel* CurrenciesRepository::Load(int id) {
	CurrencyModel* currency = nullptr;

	char* sql = "SELECT id, name, short_name, sign FROM currencies WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			currency = new CurrencyModel();

			currency->id = sqlite3_column_int(statement, 0);
			currency->name = std::wstring((wchar_t*)sqlite3_column_text16(statement, 1));
			currency->shortName = std::wstring((wchar_t*)sqlite3_column_text16(statement, 2));
			currency->sign = std::wstring((wchar_t*)sqlite3_column_text16(statement, 3));
		}
	}

	sqlite3_finalize(statement);

	return currency;
}