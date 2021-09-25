#include "pch.h"

#include "../Clerk/Data/DataContext.h"
#include "../Clerk/Data/Settings.h"

class Environment : public ::testing::Environment {
public:
	static Environment& Instance() {
		static Environment* instance = new Environment;
		return *instance;
	}

	~Environment() override {
		
	}
	
	void SetUp() override {
		std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");

		connection = new DataConnection(std::move(path));
		connection->Open();

		Settings::GetInstance().SetBaseCurrencyId(152);

		AccountsRepository* accountsRepository = new AccountsRepository(*connection);
		ReportsRepository* reportsRepository = new ReportsRepository(*connection);
		BudgetsRepository* budgetsRepository = new BudgetsRepository(*connection);
		GoalsRepository* goalsRepository = new GoalsRepository(*connection);
		AlertsRepository* alertsRepository = new AlertsRepository(*connection);
		SchedulersRepository* schedulersRepository = new SchedulersRepository(*connection);
		TransactionsRepository* transactionsRepository = new TransactionsRepository(*connection);
		TagsRepository* tagsRepository = new TagsRepository(*connection);
		CurrenciesRepository* currenciesRepository = new CurrenciesRepository(*connection);
		ExchangeRatesRepository* exchangeRatesRepository = new ExchangeRatesRepository(*connection);
		ReportingRepository* reportingRepository = new ReportingRepository(*connection);

		AccountingService* accountingService = new AccountingService(*accountsRepository, *exchangeRatesRepository);
		AccountsService* accountsService = new AccountsService(*accountsRepository, *currenciesRepository);
		SchedulersService* schedulersService = new SchedulersService(*schedulersRepository, *accountsRepository, *exchangeRatesRepository);
		BudgetsService* budgetsService = new BudgetsService(*budgetsRepository);
		TagsService* tagsService = new TagsService(*tagsRepository);
		TransactionsService* transactionsService = new TransactionsService(*transactionsRepository, *accountsService, *tagsService);
		ReportsService* reportsService = new ReportsService(*reportsRepository);
		AlertsService* alertsService = new AlertsService(*alertsRepository);
		GoalsService* goalsService = new GoalsService(*goalsRepository);
		ReportingService* reportingService = new ReportingService(*reportingRepository, *exchangeRatesRepository);

		currenciesRepository->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		accountingService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		schedulersService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		reportingService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());

		_context = new DataContext(*accountsRepository, *reportsRepository, *budgetsRepository, *goalsRepository, *alertsRepository,
			*schedulersRepository, *transactionsRepository, *tagsRepository, *currenciesRepository, *exchangeRatesRepository, *reportingRepository,
			*accountingService, *accountsService, *schedulersService, *budgetsService, *transactionsService, *tagsService,
			*reportsService, *alertsService, *goalsService, *reportingService);
	}

	void TearDown() override {
		connection->Close();
	}

	DataContext* GetContext() {
		return _context;
	}

private:
	DataContext* _context;

protected:
	DataConnection* connection;	
};