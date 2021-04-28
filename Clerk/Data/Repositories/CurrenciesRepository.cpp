#include "CurrenciesRepository.h"

using namespace Clerk::Data;

std::vector<std::shared_ptr<Currency>> CurrenciesRepository::GetAll() {
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

std::shared_ptr<Currency> CurrenciesRepository::GetById(int id) {
	std::shared_ptr<Currency> currency = GetFromHash(id);

	if (!currency) {
		currency = Load(id);
		AddToHash(id, currency);
	}

	return currency;
}

void CurrenciesRepository::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

std::shared_ptr<Currency> CurrenciesRepository::GetBaseCurrency() {
	return GetById(_baseCurrencyId);
}

std::shared_ptr<Currency> CurrenciesRepository::Load(int id) {
	std::shared_ptr<Currency> currency = nullptr;

	char* sql = "SELECT id, name, short_name, sign FROM currencies WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			currency = std::make_shared<Currency>();

			currency->id = sqlite3_column_int(statement, 0);
			currency->name = std::make_shared<wxString>(wxString::FromUTF8((char*)sqlite3_column_text(statement, 1)));
			currency->shortName = std::make_shared<wxString>(wxString::FromUTF8((char*)sqlite3_column_text(statement, 2)));
			currency->sign = std::make_shared<wxString>(wxString::FromUTF8((char*)sqlite3_column_text(statement, 3)));
		}
	}

	sqlite3_finalize(statement);

	return currency;
}