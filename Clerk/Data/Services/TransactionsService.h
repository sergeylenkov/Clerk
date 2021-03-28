#pragma once

#include "../Repositories/TransactionsRepository.h"
#include "../ViewModels/TransactionViewModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class TransactionsService {
		public:
			TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService);

			std::shared_ptr<TransactionViewModel> GetById(int id);
			std::vector<std::shared_ptr<TransactionViewModel>> GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			std::vector<std::shared_ptr<TransactionViewModel>> GetRecents(int count);
			std::vector<std::shared_ptr<TransactionViewModel>> GetRecents(const AccountViewModel& account, int count);
			std::vector<std::shared_ptr<TransactionViewModel>> GetDeleted();

		private:			
			TransactionsRepository& _transactionsRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;

			void LoadDetails(TransactionViewModel& model, TransactionModel& transaction);
		};
	}
}


