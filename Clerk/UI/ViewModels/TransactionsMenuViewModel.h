#pragma once

#include "../../Data/Services/TransactionsService.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionsMenuViewModel {
		public:
			TransactionsMenuViewModel(TransactionsService& transactionsService);
			~TransactionsMenuViewModel();

			shared_vector<TransactionPresentationModel> GetRecents();

			void OnUpdate(std::function<void()> fn);

		private:
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;
		};
	}
}