#pragma once

#include "../Services/AccountingService.h"
#include "../Services/TransactionsService.h"
#include "../Services/AccountsService.h"
#include "../Services/BudgetsService.h"
#include "../Services/SchedulersService.h"
#include "../Services/GoalsService.h"
#include "../Models/Currency.h"
#include "../../Utils/EventEmitter.h"

namespace Clerk {
	namespace Data {
		class DashboardViewModel {
		public:
			DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService,
				BudgetsService& budgetsService, SchedulersService& schedulersService, GoalsService& goalsService, Currency& currency);
			~DashboardViewModel();

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

			void OnUpdate(std::function<void()> fn);

		private:
			AccountingService& _accountingService;
			TransactionsService& _transactionsService;
			AccountsService& _accountsService;
			BudgetsService& _budgetsService;
			SchedulersService& _schedulersService;
			GoalsService& _goalsService;
			Currency& _currency;
			EventEmitter* _eventEmitter;
		};
	}
}