#pragma once

#include "../../Data/Services/TransactionsService.h"
#include "../../Utils/EventEmitter/EventEmitter.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class MainMenuViewModel {
		public:
			MainMenuViewModel(TransactionsService& transactionsService);
			~MainMenuViewModel();

			shared_vector<TransactionPresentationModel> GetRecents();

			void OnUpdate(std::function<void()> fn);

		private:
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;
		};
	}
}