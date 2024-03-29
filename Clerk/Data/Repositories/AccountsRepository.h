#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "BaseRepository.h"
#include "../Models/AccountModel.h"

namespace Clerk {
	namespace Data {
		class AccountsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			AccountModel* GetById(int id);
			std::vector<AccountModel*> GetAll();
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
			AccountModel* Load(int id);
		};
	}
}