#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "BaseRepository.h"
#include "../Models/TransactionModel.h"
#include "../Models/AccountModel.h"

namespace Clerk {
	namespace Data {
		class TransactionsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			TransactionModel* GetById(int id);
			std::vector<TransactionModel*> GetAll();
			std::vector<TransactionModel*> GetDeleted();
			std::vector<TransactionModel*> GetRecents(int count);
			std::vector<TransactionModel*> GetRecents(int accountId, int count);
			std::vector<TransactionModel*> GetForPeriod(std::string& fromDate, std::string& toDate);
			TransactionModel* GetInitialTransactionForAccount(int accountId);

			int Save(const TransactionModel& transaction);
			void Delete(const TransactionModel& transaction);

		private:
			TransactionModel* Load(int id);
			void UpdateTags(const TransactionModel& transaction);
		};
	}
}

