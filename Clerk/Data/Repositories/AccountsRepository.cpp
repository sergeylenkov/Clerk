#include "AccountsRepository.h"

using namespace Clerk::Data;

std::vector<std::shared_ptr<AccountModel>> AccountsRepository::GetAll() {
	char* sql = "SELECT id FROM accounts a";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);

			if (!GetFromHash(id)) {
				auto account = Load(id);
				AddToHash(id, account);
			}
		}
	}

	sqlite3_finalize(statement);

	return GetHashList();
}

std::shared_ptr<AccountModel> AccountsRepository::GetById(int id) {
	std::shared_ptr<AccountModel> account = GetFromHash(id);
	
	if (!account) {
		account = Load(id);
		AddToHash(id, account);
	}

	return account;
}

std::vector<std::shared_ptr<AccountModel>> AccountsRepository::GetActive() {
	auto accounts = GetAll();
	std::vector<std::shared_ptr<AccountModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [](const std::shared_ptr<AccountModel>& account) {
		return account->isActive;
	});

	return result;
}

std::vector<std::shared_ptr<AccountModel>> AccountsRepository::GetByType(AccountType type) {
	auto accounts = GetAll();
	std::vector<std::shared_ptr<AccountModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [type](const std::shared_ptr<AccountModel>& account) {
		return account->isActive && account->type == type;
	});

	return result;
}

std::vector<std::shared_ptr<AccountModel>> AccountsRepository::GetArchive() {
	auto accounts = GetAll();
	std::vector<std::shared_ptr<AccountModel>> result;

	std::copy_if(accounts.begin(), accounts.end(), std::back_inserter(result), [](const std::shared_ptr<AccountModel>& account) {
		return !account->isActive;
	});

	return result;
}

float AccountsRepository::GetBalance(int accountId, AccountType type)
{
	float receipt = 0.0;
	float expense = 0.0;

	char* sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0";

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	float total;

	if (type == AccountType::Receipt || type == AccountType::Expens) {
		total = expense - receipt;
	}
	else {
		total = receipt - expense;
	}

	return total;
}

float AccountsRepository::GetBalanceForDate(int accountId, AccountType type, std::string& date) {
	float receipt = 0.0;
	float expense = 0.0;

	char* sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0 AND paid_at <= ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, date.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0 paid_at <= ?";

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, date.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	float total;

	if (type == AccountType::Receipt || type == AccountType::Expens) {
		total = expense - receipt;
	}
	else {
		total = receipt - expense;
	}

	return total;
}

float AccountsRepository::GetExpenses(int accountId) {
	float expense = 0.0;

	char* sql = "SELECT TOTAL(from_account_amount) FROM transactions WHERE from_account_id = ? AND deleted = 0";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			expense = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	return expense;
}

float AccountsRepository::GetReceipts(int accountId) {
	float receipt = 0;

	char* sql = "SELECT TOTAL(to_account_amount) FROM transactions WHERE to_account_id = ? AND deleted = 0";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			receipt = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	return receipt;
}

float AccountsRepository::GetExpenses(int accountId, std::string& fromDate, std::string& toDate) {
	float total = 0;

	char* sql = "SELECT TOTAL(t.to_account_amount) FROM transactions t, accounts a WHERE (a.type_id = 2 OR a.type_id = 3) AND t.to_account_id = ? AND t.to_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, toDate.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	return total;
}

float AccountsRepository::GetReceipts(int accountId, std::string& fromDate, std::string& toDate) {
	float total = 0;

	char* sql = "SELECT TOTAL(t.from_account_amount) FROM transactions t, accounts a WHERE a.type_id = 0 AND t.from_account_id = ? AND t.from_account_id = a.id AND t.paid_at >= ? AND t.paid_at <= ? AND t.deleted = 0";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, fromDate.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, toDate.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			total = static_cast<float>(sqlite3_column_double(statement, 0));
		}
	}

	sqlite3_reset(statement);

	return total;
}

int AccountsRepository::GetPairAccountId(int accountId, AccountType type, std::string& fromDate) {
	int id = -1;
	char* sql = "";

	if (type == AccountType::Deposit || type == AccountType::Receipt || type == AccountType::Virtual) {
		sql = "SELECT t.to_account_id, COUNT(*) FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? GROUP BY t.to_account_id ORDER BY COUNT(*) DESC LIMIT 1";
	}
	else if (type == AccountType::Expens || type == AccountType::Debt) {
		sql = "SELECT t.from_account_id, COUNT(*) FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 AND t.paid_at >= ? GROUP BY t.to_account_id ORDER BY COUNT(*) DESC LIMIT 1";
	}

	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, accountId);
		sqlite3_bind_text(statement, 2, fromDate.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			id = static_cast<int>(sqlite3_column_int(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	if (id == -1) {
		if (type == AccountType::Deposit || type == AccountType::Receipt || type == AccountType::Virtual) {
			sql = "SELECT t.to_account_id FROM transactions t WHERE t.from_account_id = ? AND t.deleted = 0 ORDER BY t.paid_at DESC LIMIT 1";
		}
		else if (type == AccountType::Expens || type == AccountType::Debt) {
			sql = "SELECT t.from_account_id FROM transactions t WHERE t.to_account_id = ? AND t.deleted = 0 ORDER BY t.paid_at DESC LIMIT 1";
		}

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_int(statement, 1, accountId);

			if (sqlite3_step(statement) == SQLITE_ROW) {
				id = static_cast<int>(sqlite3_column_int(statement, 0));
			}
		}

		sqlite3_finalize(statement);
	}

	return id;
}

int AccountsRepository::GetLastUsedAccountId(std::string& fromDate) {
	int id = -1;
	char* sql = "SELECT t.from_account_id, COUNT(*) FROM transactions t WHERE t.deleted = 0 AND t.paid_at >= ? GROUP BY t.from_account_id ORDER BY COUNT(*) DESC LIMIT 1";	

	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fromDate.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(statement) == SQLITE_ROW) {
			id = static_cast<int>(sqlite3_column_int(statement, 0));
		}
	}

	sqlite3_finalize(statement);

	return id;
}

std::shared_ptr<AccountModel> AccountsRepository::Load(int id) {
	std::shared_ptr<AccountModel> account = nullptr;

	char* sql = "SELECT id, name, note, icon_id, type_id, order_id, currency_id, credit_limit, active, created_at FROM accounts WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			account = std::make_shared<AccountModel>();

			account->id = sqlite3_column_int(statement, 0);
			account->name = std::string((char*)sqlite3_column_text(statement, 1));
			account->note = std::string((char*)sqlite3_column_text(statement, 2));
			account->iconId = sqlite3_column_int(statement, 3);
			account->type = static_cast<AccountType>(sqlite3_column_int(statement, 4));
			account->orderId = sqlite3_column_int(statement, 5);
			account->currencyId = sqlite3_column_int(statement, 6);
			account->creditLimit = static_cast<float>(sqlite3_column_double(statement, 7));
			account->isActive = sqlite3_column_int(statement, 8);
			account->created = std::string((char*)sqlite3_column_text(statement, 9));
		}
	}

	sqlite3_finalize(statement);

	return account;
}

void AccountsRepository::Save(AccountModel& account) {
	if (account.id == -1) {
		char* sql = "INSERT INTO accounts (name, note, type_id, icon_id, order_id, currency_id, active, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, account.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, account.note.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(account.type));
			sqlite3_bind_int(statement, 4, account.iconId);
			sqlite3_bind_int(statement, 5, account.orderId);
			sqlite3_bind_int(statement, 6, account.currencyId);
			sqlite3_bind_int(statement, 7, true);
			sqlite3_bind_text(statement, 8, account.created.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				account.id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		char* sql = "UPDATE accounts SET name = ?, note = ?, type_id = ?, icon_id = ?, order_id = ?, currency_id = ?, active = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, account.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(statement, 2, account.note.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 3, static_cast<int>(account.type));
			sqlite3_bind_int(statement, 4, account.iconId);
			sqlite3_bind_int(statement, 5, account.orderId);
			sqlite3_bind_int(statement, 6, account.currencyId);
			sqlite3_bind_int(statement, 7, account.isActive);
			sqlite3_bind_int(statement, 8, account.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
	}
}

void AccountsRepository::Delete(const AccountModel& account) {
	char* sql = "DELETE FROM accounts WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, account.id);

		if (sqlite3_step(statement) == SQLITE_DONE) {
			RemoveFromHash(account.id);
		}
	}

	sqlite3_finalize(statement);
}