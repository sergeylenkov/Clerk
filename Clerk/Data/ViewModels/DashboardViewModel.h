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

		private:
			AccountingService& _accountingService;
			Currency& _currency;
		};
	}
}