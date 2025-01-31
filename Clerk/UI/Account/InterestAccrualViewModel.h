#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Utils/Types.h"
#include "Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class InterestAccrualViewModel {
		public:
			InterestAccrualViewModel(AccountsService& accountsService, TransactionsService& transactionsService);

			void SetAccountId(int id);			
			void SetAmount(float amount);
			float GetAmount();			

			void Save();
			std::function<void(InterestAccrualViewModelField field)> OnUpdate;

		private:
			AccountsService& _accountsService;
			TransactionsService& _transactionsService;
			int _id;
			float _amount;
		};
	}
}