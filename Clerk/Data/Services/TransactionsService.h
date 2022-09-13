#pragma once

#include "../Repositories/TransactionsRepository.h"
#include "../ViewModels/TransactionViewModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"
#include "../../Utils/EventEmitter.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class TransactionsService {
		public:
			TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService);
			~TransactionsService();

			std::shared_ptr<TransactionViewModel> GetById(int id);
			std::vector<std::shared_ptr<TransactionViewModel>> GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			std::vector<std::shared_ptr<TransactionViewModel>> GetRecents(int count);
			std::vector<std::shared_ptr<TransactionViewModel>> GetRecents(const AccountViewModel& account, int count);
			std::vector<std::shared_ptr<TransactionViewModel>> GetDeleted();
			void Duplicate(TransactionViewModel& viewModel);
			void Split(TransactionViewModel& splitTransaction, TransactionViewModel& newTransaction);
			void Save(TransactionViewModel& viewModel);
			void Delete(TransactionViewModel& viewModel);

			void OnUpdate(std::function<void()> fn);

		private:			
			TransactionsRepository& _transactionsRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			EventEmitter* _eventEmitter;

			void LoadDetails(TransactionViewModel& model, TransactionModel& transaction);
		};
	}
}


