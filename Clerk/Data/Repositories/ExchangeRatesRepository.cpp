#include "ExchangeRatesRepository.h"

using namespace Clerk::Data;

float ExchangeRatesRepository::GetExchangeRate(int fromCurrencyId, int toCurrencyId) {
	if (_hash.empty()) {
		Load();
	}

	auto pair = std::make_pair(fromCurrencyId, toCurrencyId);

	auto search = _hash.find(pair);

	if (search != _hash.end()) {
		return search->second;
	}

	return 1;
}

void ExchangeRatesRepository::Load() {
	char* sql = "SELECT from_currency_id, to_currency_id, rate, count, MAX(date) FROM exchange_rates GROUP BY from_currency_id, to_currency_id";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int fromId = sqlite3_column_int(statement, 0);
			int toId = sqlite3_column_int(statement, 1);
			float rate = static_cast<float>(sqlite3_column_double(statement, 2));
			int count = sqlite3_column_int(statement, 3);

			_hash[std::make_pair(fromId, toId)] = count * rate;
			_hash[std::make_pair(toId, fromId)] = count / rate;
		}
	}

	sqlite3_finalize(statement);
}