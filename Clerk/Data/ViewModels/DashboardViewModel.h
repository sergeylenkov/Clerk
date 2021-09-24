#pragma once

#include "../Services/AccountingService.h"
#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		class DashboardViewModel {
		public:
			DashboardViewModel(AccountingService& accountingService, Currency& currency);

			float GetTotalFunds();
			float GetOwnFunds();
			float GetCreditFunds();
			Currency& GetCurrency();

			std::function<void()> OnUpdate;

		private:
			AccountingService& _accountingService;
			Currency& _currency;
		};
	}
}