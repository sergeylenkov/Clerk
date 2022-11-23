#include "pch.h"

#include "../Clerk/Data/DataContext.h"
#include "../Clerk/Data/Settings.h"

class Fixture : public ::testing::Test {
public:
	Fixture() {
		SetUpConnection();
		SetUpContext();
	}

	~Fixture() {

	}

	void SetUpConnection() {
		std::string path("Test.sqlite");

		connection = new DataConnection(std::move(path));
	}

	void SetUpContext() {
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

		CurrenciesService* currenciesService = new CurrenciesService(*currenciesRepository);
		AccountingService* accountingService = new AccountingService(*accountsRepository, *exchangeRatesRepository);
		AccountsService* accountsService = new AccountsService(*accountsRepository, *currenciesService);
		SchedulersService* schedulersService = new SchedulersService(*schedulersRepository, *accountsRepository, *exchangeRatesRepository);
		BudgetsService* budgetsService = new BudgetsService(*budgetsRepository);
		TagsService* tagsService = new TagsService(*tagsRepository);
		TransactionsService* transactionsService = new TransactionsService(*transactionsRepository, *accountsService, *tagsService);
		ReportsService* reportsService = new ReportsService(*reportsRepository);
		AlertsService* alertsService = new AlertsService(*alertsRepository);
		GoalsService* goalsService = new GoalsService(*goalsRepository);
		ReportingService* reportingService = new ReportingService(*reportingRepository, *exchangeRatesRepository);

		currenciesService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		accountingService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		schedulersService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());
		reportingService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());

		context = new DataContext(*accountsRepository, *reportsRepository, *budgetsRepository, *goalsRepository, *alertsRepository,
			*schedulersRepository, *transactionsRepository, *tagsRepository, *currenciesRepository, *exchangeRatesRepository, *reportingRepository,
			*accountingService, *accountsService, *schedulersService, *budgetsService, *transactionsService, *tagsService,
			*reportsService, *alertsService, *goalsService, *reportingService, *currenciesService);
	}

	void SetUp() override {
		SetUpConnection();
		SetUpContext();
	}

	void TearDown() override {
		delete context;
		delete connection;
	}

protected:
	DataContext* context;
	DataConnection* connection;
};