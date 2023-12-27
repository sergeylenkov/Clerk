#pragma once

#include "../../Data/Services/TransactionsService.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class NewTransactionViewModel {
		public:
			NewTransactionViewModel(TransactionsService& transactionsService);
			~NewTransactionViewModel();

			shared_vector<TransactionPresentationModel> GetRecents();

			void OnUpdate(std::function<void()> fn);

		private:
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;
		};
	}
}