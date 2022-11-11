#pragma once

#include "../Services/TransactionsService.h"
#include "TransactionViewModel.h"

namespace Clerk {
	namespace Data {
		class TransactionsMenuViewModel {
		public:
			TransactionsMenuViewModel(TransactionsService& transactionsService);
			~TransactionsMenuViewModel();

			std::vector<std::shared_ptr<TransactionViewModel>> GetRecents();

			void OnUpdate(std::function<void()> fn);

		private:
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
		};
	}
}