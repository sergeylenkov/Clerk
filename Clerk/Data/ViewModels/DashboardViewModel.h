#pragma once

#include "../Services/AccountingService.h"
#include "../Services/TransactionsService.h"
#include "../Services/AccountsService.h"
#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		class DashboardViewModel {
		public:
			DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService, Currency& currency);

			float GetTotalFunds();
			float GetOwnFunds();
			float GetCreditFunds();
			Currency& GetCurrency();
			std::vector<std::shared_ptr<AccountViewModel>> GetAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetExpensesForMonth();
			float GetTotalExpensesForMonth();

			std::function<void()> OnUpdate;

		private:
			AccountingService& _accountingService;
			TransactionsService& _transactionsService;
			AccountsService& _accountsService;
			Currency& _currency;
		};
	}
}