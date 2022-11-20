#pragma once

#include "../../Data/Services/TransactionsService.h"
#include "../PresentationModels/TransactionPresentationModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionsMenuViewModel {
		public:
			TransactionsMenuViewModel(TransactionsService& transactionsService);
			~TransactionsMenuViewModel();

			std::vector<std::shared_ptr<TransactionPresentationModel>> GetRecents();

			void OnUpdate(std::function<void()> fn);

		private:
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
		};
	}
}