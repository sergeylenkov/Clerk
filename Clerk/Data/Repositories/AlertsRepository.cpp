#include "AlertsRepository.h"

using namespace Clerk::Data;

AlertModel* AlertsRepository::GetById(int id) {	
	return Load(id);
}

std::vector<AlertModel*> AlertsRepository::GetAll() {
	std::vector<AlertModel*> result;

	char* sql = "SELECT id FROM alerts ORDER BY name";
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

float AlertsRepository::GetBalance(const std::wstring& accountsIds) {
	float total = 0.0;
	float receipt_sum = 0.0;
	float expense_sum = 0.0;
	char sql[512];

	snprintf(sql, sizeof(sql), "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id IN(%ls) AND deleted = 0", accountsIds.c_str());
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt_sum = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	snprintf(sql, sizeof(sql), "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id IN(%ls) AND deleted = 0", accountsIds.c_str());

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense_sum = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	total = receipt_sum - expense_sum;

	return total;
}

AlertModel* AlertsRepository::Load(int id) {
	AlertModel* alert = nullptr;

	char* sql = "SELECT id, name, message, type, period, condition, importance, amount, account_ids, created_at FROM alerts WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			alert = new AlertModel();

			alert->id = sqlite3_column_int(statement, 0);
			alert->name = std::wstring((wchar_t*)sqlite3_column_text16(statement, 1));
			alert->message = std::wstring((wchar_t*)sqlite3_column_text16(statement, 2));
			alert->type = static_cast<AlertType>(sqlite3_column_int(statement, 3));
			alert->period = static_cast<AlertPeriod>(sqlite3_column_int(statement, 4));
			alert->condition = static_cast<AlertCondition>(sqlite3_column_int(statement, 5));
			alert->importance = static_cast<AlertImportance>(sqlite3_column_int(statement, 6));
			alert->amount = static_cast<float>(sqlite3_column_double(statement, 7));
			alert->accountIds = std::wstring((wchar_t*)sqlite3_column_text16(statement, 8));
			alert->created = std::wstring((wchar_t*)sqlite3_column_text16(statement, 9));
		}
	}

	sqlite3_finalize(statement);

	return alert;
}

int AlertsRepository::Save(const AlertModel& alert) {
	int id = alert.id;

	if (alert.id == -1) {
		char* sql = "INSERT INTO alerts (name, message, type, period, condition, importance, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, alert.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 2, alert.message.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(alert.type));
			sqlite3_bind_int(statement, 4, static_cast<int>(alert.period));
			sqlite3_bind_int(statement, 5, static_cast<int>(alert.condition));
			sqlite3_bind_int(statement, 6, static_cast<int>(alert.importance));
			sqlite3_bind_double(statement, 7, alert.amount);
			sqlite3_bind_text16(statement, 8, alert.accountIds.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 9, alert.created.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	}
	else {		
		char* sql = "UPDATE alerts SET name = ?, message = ?, type = ?, period = ?, condition = ?, importance = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, alert.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 2, alert.message.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(alert.type));
			sqlite3_bind_int(statement, 4, static_cast<int>(alert.period));
			sqlite3_bind_int(statement, 5, static_cast<int>(alert.condition));
			sqlite3_bind_int(statement, 6, static_cast<int>(alert.importance));
			sqlite3_bind_double(statement, 7, alert.amount);
			sqlite3_bind_text16(statement, 8, alert.accountIds.c_str(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 9, alert.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}

	return id;
}

void AlertsRepository::Delete(const AlertModel& alert)
{
	char* sql = "DELETE FROM alerts WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, alert.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}