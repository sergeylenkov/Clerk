#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "HashRepository.h"
#include "../Models/TransactionModel.h"
#include "../Models/AccountModel.h"

namespace Clerk {
	namespace Data {
		class TransactionsRepository : HashRepository<TransactionModel> {
		public:
			using HashRepository::HashRepository;

			std::shared_ptr<TransactionModel> GetById(int id);
			std::vector<std::shared_ptr<TransactionModel>> GetAll();
			std::vector<std::shared_ptr<TransactionModel>> GetDeleted();
			std::vector<std::shared_ptr<TransactionModel>> GetRecents(int count);
			std::vector<std::shared_ptr<TransactionModel>> GetRecents(int accountId, int count);
			std::vector<std::shared_ptr<TransactionModel>> GetForPeriod(std::string& fromDate, std::string& toDate);
			std::shared_ptr<TransactionModel> GetInitialTransactionForAccount(int accountId);

			std::shared_ptr<TransactionModel> Save(std::shared_ptr<TransactionModel> transaction);
			void Delete(std::shared_ptr<TransactionModel> transaction);

		private:
			std::shared_ptr<TransactionModel> Load(int id);
			void UpdateTags(std::shared_ptr<TransactionModel> transaction);
		};
	}
}

