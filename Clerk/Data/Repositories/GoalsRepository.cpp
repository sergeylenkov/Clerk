#include "GoalsRepository.h"

using namespace Clerk::Data;

GoalModel* GoalsRepository::GetById(int id) {
	return Load(id);
}

std::vector<GoalModel*> GoalsRepository::GetAll() {
	std::vector<GoalModel*> result;

	char* sql = "SELECT id FROM goals ORDER BY name";
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


float GoalsRepository::GetBalance(std::string accountsIds) {
	float total = 0.0;
	float receipt_sum = 0.0;
	float expense_sum = 0.0;
	
	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id IN(%s) AND deleted = 0", accountsIds.c_str());	

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_reset(statement);

	snprintf(sql, sizeof(sql), "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id IN(%s) AND deleted = 0", accountsIds.c_str());

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense_sum = sqlite3_column_double(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	total = receipt_sum - expense_sum;

	return total;
}

GoalModel* GoalsRepository::Load(int id) {
	GoalModel* goal = nullptr;

	char* sql = "SELECT id, name, date, amount, account_ids, created_at FROM goals WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			goal = new GoalModel();

			goal->id = sqlite3_column_int(statement, 0);
			goal->name = std::wstring((wchar_t*)sqlite3_column_text16(statement, 1));
			goal->date = std::wstring((wchar_t*)sqlite3_column_text16(statement, 2));
			goal->amount = static_cast<float>(sqlite3_column_double(statement, 3));
			goal->accountIds = std::wstring((wchar_t*)sqlite3_column_text16(statement, 4));
			goal->created = std::wstring((wchar_t*)sqlite3_column_text16(statement, 5));
		}
	}

	sqlite3_finalize(statement);

	return goal;
}

int GoalsRepository::Save(const GoalModel& goal) {
	int id = goal.id;

	if (goal.id == -1) {
		char* sql = "INSERT INTO goals (name, date, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, goal.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 2, goal.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, goal.amount);
			sqlite3_bind_text16(statement, 4, goal.accountIds.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 5, goal.created.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	} else {
		char* sql = "UPDATE goals SET name = ?, date = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, goal.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 2, goal.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, goal.amount);
			sqlite3_bind_text16(statement, 4, goal.accountIds.c_str(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 5, goal.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}

	return id;
}

void GoalsRepository::Delete(const GoalModel& goal)
{
	char* sql = "DELETE FROM goals WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, goal.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}