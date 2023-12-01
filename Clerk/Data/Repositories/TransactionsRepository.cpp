#include "TransactionsRepository.h"

using namespace Clerk::Data;

TransactionModel* TransactionsRepository::GetById(int id) {
	return Load(id);
}

std::vector<TransactionModel*> TransactionsRepository::GetAll() {
	std::vector<TransactionModel*> result;

	char* sql = "SELECT id FROM transactions ORDER BY id";
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

std::vector<TransactionModel*> TransactionsRepository::GetDeleted() {
	std::vector<TransactionModel*> result;

	char* sql = "SELECT id FROM transactions WHERE deleted = 1 ORDER BY paid_at DESC, created_at DESC";
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

std::vector<TransactionModel*> TransactionsRepository::GetRecents(int count) {
	std::vector<TransactionModel*> result;

	char* sql = "SELECT MAX(paid_at), id FROM transactions GROUP BY from_account_id, to_account_id ORDER BY paid_at DESC LIMIT ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, count);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 1);
			result.push_back(Load(id));
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<TransactionModel*> TransactionsRepository::GetRecents(int accountId, int count) {
	std::vector<TransactionModel*> result;

	char* sql = "SELECT id FROM\
		(\
			SELECT t.paid_at, t.id, t.from_account_id, t.to_account_id, MAX(tt.tag_id) AS tag_id\
			FROM transactions t, transactions_tags tt\
			WHERE(t.from_account_id = ? OR t.to_account_id = ?) AND t.id = tt.transaction_id\
			GROUP BY t.from_account_id, t.to_account_id, t.id\
			ORDER BY t.paid_at DESC, t.id DESC\
		)\
		GROUP BY from_account_id, to_account_id, tag_id\
		ORDER BY paid_at DESC, id DESC\
		LIMIT ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_int(statement, 2, accountId);
		sqlite3_bind_int(statement, 3, count);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);
			result.push_back(Load(id));
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::vector<TransactionModel*> TransactionsRepository::GetForPeriod(std::string& fromDate, std::string& toDate) {
	std::vector<TransactionModel*> result;

	char* sql = "SELECT t.id FROM transactions t, accounts fa, accounts ta \
				  WHERE t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0 AND fa.id = t.from_account_id AND ta.id = t.to_account_id ORDER BY t.paid_at DESC, t.created_at DESC";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 2, toDate.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);
			result.push_back(Load(id));
		}
	}

	sqlite3_finalize(statement);

	return result;
}

TransactionModel* TransactionsRepository::GetInitialTransactionForAccount(int accountId) {
	TransactionModel* transaction = nullptr;

	char* sql = "SELECT id FROM transactions WHERE from_account_id = ? AND to_account_id = -1";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			transaction = Load(sqlite3_column_int(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	return transaction;
}

TransactionModel* TransactionsRepository::Load(int id) {
	TransactionModel* transaction = nullptr;

	char* sql = "SELECT id, from_account_id, to_account_id, from_account_amount, to_account_amount, paid_at, note FROM transactions WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			transaction = new TransactionModel();

			transaction->id = sqlite3_column_int(statement, 0);
			transaction->fromAccountId = sqlite3_column_int(statement, 1);
			transaction->toAccountId = sqlite3_column_int(statement, 2);
			transaction->fromAmount = static_cast<float>(sqlite3_column_double(statement, 3));
			transaction->toAmount = static_cast<float>(sqlite3_column_double(statement, 4));
			transaction->date = std::wstring((wchar_t*)sqlite3_column_text16(statement, 5));
			transaction->note = std::wstring((wchar_t*)sqlite3_column_text16(statement, 6));
		}
	}

	sqlite3_finalize(statement);	

	if (transaction) {
		sql = "SELECT tag_id FROM transactions_tags WHERE transaction_id = ?";

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, id);

			while (sqlite3_step(statement) == SQLITE_ROW) {
				transaction->tagsIds.push_back(sqlite3_column_int(statement, 0));
			}
		}

		sqlite3_finalize(statement);
	}

	return transaction;
}

int TransactionsRepository::Save(const TransactionModel& transaction) {
	int id = transaction.id;

	if (transaction.id == -1) {
		char* sql = "INSERT INTO transactions (from_account_id, to_account_id, from_account_amount, to_account_amount, deleted, paid_at, note, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, transaction.fromAccountId);
			sqlite3_bind_int(statement, 2, transaction.toAccountId);
			sqlite3_bind_double(statement, 3, transaction.fromAmount);
			sqlite3_bind_double(statement, 4, transaction.toAmount);
			sqlite3_bind_int(statement, 5, false);
			sqlite3_bind_text16(statement, 6, transaction.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 7, transaction.note.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 8, transaction.created.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));

				UpdateTags(id, transaction.tagsIds);
			}
		}

		sqlite3_finalize(statement);	
	}
	else {		
		char* sql = "UPDATE transactions SET from_account_id = ?, to_account_id = ?, from_account_amount = ?, to_account_amount = ?, paid_at = ?, note = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, transaction.fromAccountId);
			sqlite3_bind_int(statement, 2, transaction.toAccountId);
			sqlite3_bind_double(statement, 3, transaction.fromAmount);
			sqlite3_bind_double(statement, 4, transaction.toAmount);
			sqlite3_bind_text16(statement, 5, transaction.date.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text16(statement, 6, transaction.note.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 7, transaction.id);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				UpdateTags(id, transaction.tagsIds);
			}
		}

		sqlite3_finalize(statement);
	}

	return id;
}

void TransactionsRepository::Delete(const TransactionModel& transaction) {
	char* sql = "DELETE FROM transactions WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, transaction.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "DELETE FROM transactions_tags WHERE transaction_id = ?";

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, transaction.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}

void TransactionsRepository::UpdateTags(int id, std::vector<int> tagsIds)
{
	char* sql = "DELETE FROM transactions_tags WHERE transaction_id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	for (int tagId : tagsIds) {
		sql = "INSERT INTO transactions_tags (tag_id, transaction_id) VALUES (?, ?)";

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, tagId);
			sqlite3_bind_int(statement, 2, id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
}