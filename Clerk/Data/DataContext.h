#pragma once

#include "./Repositories/AccountsRepository.h"
#include "./Repositories/ReportsRepository.h"
#include "./Repositories/BudgetsRepository.h"
#include "./Repositories/GoalsRepository.h"
#include "./Repositories/AlertsRepository.h"
#include "./Repositories/SchedulersRepository.h"
#include "./Repositories/TransactionsRepository.h"
#include "./Repositories/TagsRepository.h"
#include "./Repositories/CurrenciesRepository.h"
#include "./Repositories/ReportingRepository.h"
#include "./Services/AccountingService.h"
#include "./Services/AccountsService.h"
#include "./Services/SchedulersService.h"
#include "./Services/BudgetsService.h"
#include "./Services/TransactionsService.h"
#include "./Services/TagsService.h"
#include "./Services/ReportsService.h"
#include "./Services/AlertsService.h"
#include "./Services/GoalsService.h"
#include "./Services/ReportingService.h"
#include "./Services/CurrenciesService.h"
#include "../Commands/CommandsInvoker.h"

using namespace Clerk::Commands;

namespace Clerk {
	namespace Data {
		class DataContext {
		public:
			DataContext(AccountsRepository& accountsRepository, ReportsRepository& reportsRepository, BudgetsRepository& budgetsRepository,
				GoalsRepository& goalsRepository, AlertsRepository& alertsRepository, SchedulersRepository& schedulersRepository,
				TransactionsRepository& transactionsRepository, TagsRepository& tagsRepository, CurrenciesRepository& currenciesRepository,
				ReportingRepository& reportingRepository, AccountingService& accountingService, AccountsService& accountsService, SchedulersService& schedulersService, 
				BudgetsService& budgetsService, TransactionsService& transactionsService, TagsService& tagsService, ReportsService& reportsService, 
				AlertsService& alertsService, GoalsService& goalsService, ReportingService& reportingService, CurrenciesService& currenciesService);
			~DataContext();

			AccountsRepository& GetAccountsRepository();
			ReportsRepository& GetReportsRepository();
			BudgetsRepository& GetBudgetsRepository();
			GoalsRepository& GetGoalsRepository();
			AlertsRepository& GetAlertsRepository();
			SchedulersRepository& GetSchedulersRepository();
			TransactionsRepository& GetTransactionsRepository();
			TagsRepository& GetTagsRepository();
			CurrenciesRepository& GetCurrenciesRepository();
			ReportingRepository& GetReportingRepository();
			AccountingService& GetAccountingService();
			AccountsService& GetAccountsService();
			SchedulersService& GetSchedulersService();
			BudgetsService& GetBudgetsService();
			TransactionsService& GetTransactionsService();
			TagsService& GetTagsService();
			ReportsService& GetReportsService();
			AlertsService& GetAlertsService();
			GoalsService& GetGoalsService();
			ReportingService& GetReportingService();
			CurrenciesService& GetCurrenciesService();
			void SetCommandsInvoker(CommandsInvoker* commandsInvoker);
			CommandsInvoker& GetCommandsInvoker();

		private:
			AccountsRepository& _accountsRepository;
			ReportsRepository& _reportsRepository;
			BudgetsRepository& _budgetsRepository;
			GoalsRepository& _goalsRepository;
			AlertsRepository& _alertsRepository;
			SchedulersRepository& _schedulersRepository;
			TransactionsRepository& _transactionsRepository;
			TagsRepository& _tagsRepository;
			CurrenciesRepository& _currenciesRepository;
			ReportingRepository& _reportingRepository;
			AccountingService& _accountingService;
			AccountsService& _accountsService;
			SchedulersService& _schedulersService;
			BudgetsService& _budgetsService;
			TransactionsService& _transactionsService;
			TagsService& _tagsService;
			ReportsService& _reportsService;
			AlertsService& _alertsService;
			GoalsService& _goalsService;
			ReportingService& _reportingService;
			CurrenciesService& _currenciesService;
			CommandsInvoker* _commandsInvoker;
		};
	}
}