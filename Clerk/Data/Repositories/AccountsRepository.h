#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "HashRepository.h"
#include "../Models/AccountModel.h"

namespace Clerk {
	namespace Data {
		class AccountsRepository : BaseRepository {
		public:
			BaseRepository::BaseRepository;

			std::vector<std::shared_ptr<AccountModel>> GetAll();
			std::shared_ptr<AccountModel> GetById(int id);
			std::vector<std::shared_ptr<AccountModel>> GetActive();
			std::vector<std::shared_ptr<AccountModel>> GetByType(AccountType type);
			std::vector<std::shared_ptr<AccountModel>> GetArchive();
			float GetBalance(int accountId, AccountType type);
			float GetBalanceForDate(int accountId, AccountType type, std::string& date);
			float GetExpenses(int accountId);
			float GetReceipts(int accountId);
			float GetExpenses(int accountId, std::string& fromDate, std::string& toDate);
			float GetReceipts(int accountId, std::string& fromDate, std::string& toDate);
			float GetInitialAmount(int accountId, AccountType type);
			int GetPairAccountId(int accountId, AccountType type, std::string& fromDate);
			int GetLastUsedAccountId(std::string& fromDate);

			int Save(const AccountModel& account);
			void Delete(const AccountModel& account);

		private:
			std::shared_ptr<AccountModel> Load(int id);
		};
	}
}