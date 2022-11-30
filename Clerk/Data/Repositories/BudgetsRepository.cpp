#include "BudgetsRepository.h"

using namespace Clerk::Data;

std::vector<std::shared_ptr<BudgetModel>> BudgetsRepository::GetAll() {
	if (GetHashList().empty()) {
		char* sql = "SELECT id FROM budgets ORDER BY name";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			while (sqlite3_step(statement) == SQLITE_ROW) {
				int id = sqlite3_column_int(statement, 0);
				auto budget = Load(id);

				AddToHash(id, budget);
			}
		}

		sqlite3_finalize(statement);
	}

	return GetHashList();
}

std::shared_ptr<BudgetModel> BudgetsRepository::GetById(int id) {
	std::shared_ptr<BudgetModel> budget = GetFromHash(id);

	if (!budget) {		
		budget = Load(id);
		AddToHash(id, budget);
	}

	return budget;
}

float BudgetsRepository::GetExpenses(const BudgetModel& budget, std::string& fromDate, std::string& toDate) {
	float total = 0;

	char sql[512];
	sqlite3_stmt* statement;

	snprintf(sql, sizeof(sql), "SELECT TOTAL(t.to_account_amount) FROM transactions t, accounts a WHERE (a.type_id = 2 OR a.type_id = 3) AND a.active = 1 AND t.to_account_id IN(%ls) AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0", budget.accountIds.c_str());

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	return total;
}

std::shared_ptr<BudgetModel> BudgetsRepository::Load(int id) {
	std::shared_ptr<BudgetModel> budget = nullptr;

	char* sql = "SELECT b.id, b.name, b.period, b.date, b.amount, b.account_ids FROM budgets b WHERE b.id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			budget = std::make_shared<BudgetModel>();

			budget->id = sqlite3_column_int(statement, 0);
			budget->name = std::wstring((wchar_t*)sqlite3_column_text16(statement, 1));
			budget->period = static_cast<BudgetPeriod>(sqlite3_column_int(statement, 2));
			budget->date = std::wstring((wchar_t*)sqlite3_column_text16(statement, 3));
			budget->amount = static_cast<float>(sqlite3_column_double(statement, 4));
			budget->accountIds = std::wstring((wchar_t*)sqlite3_column_text16(statement, 5));
		}
	}

	sqlite3_finalize(statement);

	return budget;
}

void BudgetsRepository::Save(BudgetModel& budget)
{
	if (budget.id == -1) {
		char* sql = "INSERT INTO budgets (name, period, date, amount, account_ids, created_at) VALUES (?, ?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, budget.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(budget.period));
			sqlite3_bind_text16(statement, 3, budget.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_double(statement, 4, budget.amount);
			sqlite3_bind_text16(statement, 5, budget.accountIds.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 6, budget.created.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				budget.id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		char* sql = "UPDATE budgets SET name = ?, period = ?, date = ?, amount = ?, account_ids = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, budget.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(budget.period));
			sqlite3_bind_text16(statement, 3, budget.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_double(statement, 4, budget.amount);
			sqlite3_bind_text16(statement, 5, budget.accountIds.c_str(), -1, SQLITE_TRANSIENT);

			sqlite3_bind_int(statement, 6, budget.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
}

void BudgetsRepository::Delete(const BudgetModel& budget)
{
	char* sql = "DELETE FROM budgets WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, budget.id);

		if (sqlite3_step(statement) == SQLITE_DONE) {
			RemoveFromHash(budget.id);
		}
	}

	sqlite3_finalize(statement);
}