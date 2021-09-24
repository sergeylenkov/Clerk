#pragma once

#include "../Services/AccountingService.h"
#include "../Services/TransactionsService.h"
#include "../Services/AccountsService.h"
#include "../Services/BudgetsService.h"
#include "../Services/SchedulersService.h"
#include "../Services/GoalsService.h"
#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		class DashboardViewModel {
		public:
			DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService,
				BudgetsService& budgetsService, SchedulersService& schedulersService, GoalsService& goalsService, Currency& currency);

			float GetTotalFunds();
			float GetOwnFunds();
			float GetCreditFunds();
			Currency& GetCurrency();
			std::vector<std::shared_ptr<AccountViewModel>> GetAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetExpensesForMonth();
			float GetTotalExpensesForMonth();
			std::vector<std::shared_ptr<BudgetViewModel>> GetBudgets();
			std::vector<std::shared_ptr<SchedulerViewModel>> GetSchedulersForMonth();
			std::vector<std::shared_ptr<AccountViewModel>> GetDepts();
			std::vector<std::shared_ptr<GoalViewModel>> GetGoals();

			std::function<void()> OnUpdate;

		private:
			AccountingService& _accountingService;
			TransactionsService& _transactionsService;
			AccountsService& _accountsService;
			BudgetsService& _budgetsService;
			SchedulersService& _schedulersService;
			GoalsService& _goalsService;
			Currency& _currency;
		};
	}
}