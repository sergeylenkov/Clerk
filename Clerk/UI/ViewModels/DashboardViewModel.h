#pragma once

#include "../../Data/Services/AccountingService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/BudgetsService.h"
#include "../../Data/Services/SchedulersService.h"
#include "../../Data/Services/GoalsService.h"
#include "../../Data/Models/CurrencyModel.h"
#include "../../Utils/EventEmitter.h"

namespace Clerk {
	namespace UI {
		class DashboardViewModel {
		public:
			DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService,
				BudgetsService& budgetsService, SchedulersService& schedulersService, GoalsService& goalsService, CurrenciesService& currenciesService);
			~DashboardViewModel();

			float GetTotalFunds();
			float GetOwnFunds();
			float GetCreditFunds();
			std::shared_ptr<CurrencyPresentationModel> GetCurrency();
			std::vector<AccountPresentationModel*> GetAccounts();
			std::vector<AccountPresentationModel*> GetExpensesForMonth();
			float GetTotalReceiptsForMonth();
			float GetTotalExpensesForMonth();
			std::vector<std::shared_ptr<BudgetPresentationModel>> GetBudgets();
			std::vector<std::shared_ptr<SchedulerPresentationModel>> GetSchedulersForMonth();
			std::vector<AccountPresentationModel*> GetDepts();
			std::vector<std::shared_ptr<GoalPresentationModel>> GetGoals();

			void OnUpdate(std::function<void()> fn);

		private:
			AccountingService& _accountingService;
			TransactionsService& _transactionsService;
			AccountsService& _accountsService;
			BudgetsService& _budgetsService;
			SchedulersService& _schedulersService;
			GoalsService& _goalsService;
			CurrenciesService& _currenciesService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;
		};
	}
}