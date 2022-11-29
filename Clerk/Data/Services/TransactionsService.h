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
			std::shared_ptr<TransactionPresentationModel> Duplicate(std::shared_ptr<TransactionPresentationModel> viewModel);
			void Split(std::shared_ptr<TransactionPresentationModel> splitTransaction, std::shared_ptr<TransactionPresentationModel> newTransaction);
			std::shared_ptr<TransactionPresentationModel> Save(std::shared_ptr<TransactionPresentationModel> viewModel);
			void Delete(std::shared_ptr<TransactionPresentationModel> viewModel);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:			
			TransactionsRepository& _transactionsRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			EventEmitter* _eventEmitter;

			void LoadDetails(std::shared_ptr<TransactionPresentationModel> model, std::shared_ptr<TransactionModel> transaction);
		};
	}
}


