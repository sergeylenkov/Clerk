#pragma once

#include "../Repositories/TransactionsRepository.h"
#include "../../UI/PresentationModels/TransactionPresentationModel.h"
#include "../../UI/PresentationModels/AccountPresentationModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"
#include "../../Utils/EventEmitter.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class TransactionsService {
		public:
			TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService);
			~TransactionsService();

			std::shared_ptr<TransactionPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<TransactionPresentationModel>> GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			std::vector<std::shared_ptr<TransactionPresentationModel>> GetRecents(int count);
			std::vector<std::shared_ptr<TransactionPresentationModel>> GetRecents(const AccountPresentationModel& account, int count);
			std::vector<std::shared_ptr<TransactionPresentationModel>> GetDeleted();
			void Duplicate(TransactionPresentationModel& viewModel);
			void Split(TransactionPresentationModel& splitTransaction, TransactionPresentationModel& newTransaction);
			void Save(TransactionPresentationModel& viewModel);
			void Delete(TransactionPresentationModel& viewModel);

			void OnUpdate(std::function<void()> fn);

		private:			
			TransactionsRepository& _transactionsRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			EventEmitter* _eventEmitter;

			void LoadDetails(TransactionPresentationModel& model, TransactionModel& transaction);
		};
	}
}


