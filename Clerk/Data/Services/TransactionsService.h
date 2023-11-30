#pragma once

#include "../Repositories/TransactionsRepository.h"
#include "../../UI/PresentationModels/TransactionPresentationModel.h"
#include "../../UI/PresentationModels/AccountPresentationModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"
#include "../../Utils/EventEmitter.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class TransactionsService : HashService< std::shared_ptr<TransactionPresentationModel>> {
		public:
			TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService);
			~TransactionsService();

			std::shared_ptr<TransactionPresentationModel> GetById(int id);
			shared_vector<TransactionPresentationModel> GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			shared_vector<TransactionPresentationModel> GetRecents(int count);
			shared_vector<TransactionPresentationModel> GetRecents(const AccountPresentationModel& account, int count);
			shared_vector<TransactionPresentationModel> GetDeleted();
			std::shared_ptr<TransactionPresentationModel> Duplicate(const TransactionPresentationModel& transaction);
			void Split(TransactionPresentationModel& splitTransaction, const TransactionPresentationModel& newTransaction);
			std::shared_ptr<TransactionPresentationModel> Save(TransactionPresentationModel& transaction);
			void Delete(TransactionPresentationModel& transaction);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:			
			TransactionsRepository& _transactionsRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			EventEmitter* _eventEmitter;

			void LoadDetails(std::shared_ptr<TransactionPresentationModel> model, TransactionModel& transaction);
		};
	}
}


