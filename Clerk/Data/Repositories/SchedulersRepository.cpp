#include "SchedulersRepository.h"

using namespace Clerk::Data;

SchedulerModel* SchedulersRepository::GetById(int id) {
	return Load(id);
}

std::vector<SchedulerModel*> SchedulersRepository::GetAll() {
	std::vector<SchedulerModel*> result;

	char* sql = "SELECT id FROM schedulers ORDER BY name";
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

SchedulerModel* SchedulersRepository::Load(int id) {
	SchedulerModel* scheduler = nullptr;

	char* sql = "SELECT s.id, s.name, s.type, s.day, s.week, s.month, s.from_account_id, s.to_account_id, s.from_account_amount, s.to_account_amount, s.tags, s.prev_date, s.next_date, s.active FROM schedulers s WHERE s.id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			scheduler = new SchedulerModel();

			scheduler->id = sqlite3_column_int(statement, 0);
			scheduler->name = std::wstring((wchar_t*)sqlite3_column_text16(statement, 1));
			scheduler->type = static_cast<SchedulerType>(sqlite3_column_int(statement, 2));
			scheduler->day = sqlite3_column_int(statement, 3);
			scheduler->week = sqlite3_column_int(statement, 4);
			scheduler->month = sqlite3_column_int(statement, 5);
			scheduler->fromAccountId = sqlite3_column_int(statement, 6);
			scheduler->toAccountId = sqlite3_column_int(statement, 7);
			scheduler->fromAmount = static_cast<float>(sqlite3_column_double(statement, 8));
			scheduler->toAmount = static_cast<float>(sqlite3_column_double(statement, 9));
			scheduler->tags = std::wstring((wchar_t*)sqlite3_column_text16(statement, 10));
			scheduler->previousDate = std::wstring((wchar_t*)sqlite3_column_text16(statement, 11));
			scheduler->nextDate = std::wstring((wchar_t*)sqlite3_column_text16(statement, 12));
			scheduler->active = sqlite3_column_int(statement, 13);
		}
	}

	sqlite3_finalize(statement);

	return scheduler;
}

int SchedulersRepository::Save(const SchedulerModel& scheduler) {
	int id = scheduler.id;

	if (scheduler.id == -1) {
		char* sql = "INSERT INTO schedulers (name, type, day, week, month, from_account_id, to_account_id, from_account_amount, to_account_amount, tags, prev_date, next_date, active) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, scheduler.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(scheduler.type));
			sqlite3_bind_int(statement, 3, scheduler.day);
			sqlite3_bind_int(statement, 4, scheduler.week);
			sqlite3_bind_int(statement, 5, scheduler.month);
			sqlite3_bind_int(statement, 6, scheduler.fromAccountId);
			sqlite3_bind_int(statement, 7, scheduler.toAccountId);
			sqlite3_bind_double(statement, 8, scheduler.fromAmount);
			sqlite3_bind_double(statement, 9, scheduler.toAmount);
			sqlite3_bind_text16(statement, 10, scheduler.tags.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 11, scheduler.previousDate.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 12, scheduler.nextDate.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 13, scheduler.active);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	}
	else {		
		char* sql = "UPDATE schedulers SET name = ?, type = ?, day = ?, week = ?, month = ?, from_account_id = ?, to_account_id = ?, from_account_amount = ?, to_account_amount = ?, tags = ?, prev_date = ?, next_date = ?, active = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, scheduler.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, static_cast<int>(scheduler.type));
			sqlite3_bind_int(statement, 3, scheduler.day);
			sqlite3_bind_int(statement, 4, scheduler.week);
			sqlite3_bind_int(statement, 5, scheduler.month);
			sqlite3_bind_int(statement, 6, scheduler.fromAccountId);
			sqlite3_bind_int(statement, 7, scheduler.toAccountId);
			sqlite3_bind_double(statement, 8, scheduler.fromAmount);
			sqlite3_bind_double(statement, 9, scheduler.toAmount);
			sqlite3_bind_text16(statement, 10, scheduler.tags.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 11, scheduler.previousDate.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 12, scheduler.nextDate.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 13, scheduler.active);

			sqlite3_bind_int(statement, 14, scheduler.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}

	return id;
}

void SchedulersRepository::Delete(const SchedulerModel& scheduler) {
	char* sql = "DELETE FROM schedulers WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, scheduler.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}