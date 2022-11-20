#include "CurrenciesRepository.h"

using namespace Clerk::Data;

std::vector<std::shared_ptr<CurrencyModel>> CurrenciesRepository::GetAll() {
	char* sql = "SELECT id FROM currencies ORDER BY name";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);

			if (!GetFromHash(id)) {
				auto currency = Load(id);
				AddToHash(id, currency);
			}			
		}
	}

	sqlite3_finalize(statement);

	return GetHashList();
}

std::shared_ptr<CurrencyModel> CurrenciesRepository::GetById(int id) {
	std::shared_ptr<CurrencyModel> currency = GetFromHash(id);

	if (!currency) {
		currency = Load(id);
		AddToHash(id, currency);
	}

	return currency;
}

std::shared_ptr<CurrencyModel> CurrenciesRepository::Load(int id) {
	std::shared_ptr<CurrencyModel> currency = nullptr;

	char* sql = "SELECT id, name, short_name, sign FROM currencies WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			currency = std::make_shared<CurrencyModel>();

			currency->id = sqlite3_column_int(statement, 0);
			currency->name = std::string((char*)sqlite3_column_text(statement, 1));
			currency->shortName = std::string((char*)sqlite3_column_text(statement, 2));
			currency->sign = std::string((char*)sqlite3_column_text(statement, 3));
		}
	}

	sqlite3_finalize(statement);

	return currency;
}