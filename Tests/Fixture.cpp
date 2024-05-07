#include "pch.h"

#include "../Clerk/Data/DataContext.h"
#include "../Clerk/Utils/Settings/Settings.h"

class Fixture : public ::testing::Test {
public:
	Fixture() {
		SetUpConnection();
		SetUpContext();
	}

	~Fixture() {

	}

	void SetUpConnection() {
		std::string path("Assets//Test.sqlite");

		_connection = new DataConnection(std::move(path));
	}

	void SetUpContext() {
		Settings::GetInstance().SetBaseCurrencyId(152);

		AccountsRepository* accountsRepository = new AccountsRepository(*_connection);
		ReportsRepository* reportsRepository = new ReportsRepository(*_connection);
		BudgetsRepository* budgetsRepository = new BudgetsRepository(*_connection);
		GoalsRepository* goalsRepository = new GoalsRepository(*_connection);
		AlertsRepository* alertsRepository = new AlertsRepository(*_connection);
		SchedulersRepository* schedulersRepository = new SchedulersRepository(*_connection);
		TransactionsRepository* transactionsRepository = new TransactionsRepository(*_connection);
		TagsRepository* tagsRepository = new TagsRepository(*_connection);
		CurrenciesRepository* currenciesRepository = new CurrenciesRepository(*_connection);
		ReportingRepository* reportingRepository = new ReportingRepository(*_connection);

		CurrenciesService* currenciesService = new CurrenciesService(*currenciesRepository);
		TagsService* tagsService = new TagsService(*tagsRepository);
		AccountsService* accountsService = new AccountsService(*accountsRepository, *currenciesService);
		AccountingService* accountingService = new AccountingService(*accountsService, *currenciesService);
		TransactionsService* transactionsService = new TransactionsService(*transactionsRepository, *accountsService, *tagsService);
		SchedulersService* schedulersService = new SchedulersService(*schedulersRepository, *accountsService, *tagsService, *transactionsService);
		BudgetsService* budgetsService = new BudgetsService(*budgetsRepository);
		ReportsService* reportsService = new ReportsService(*reportsRepository);
		AlertsService* alertsService = new AlertsService(*alertsRepository);
		GoalsService* goalsService = new GoalsService(*goalsRepository);
		ReportingService* reportingService = new ReportingService(*reportingRepository, *currenciesService);

		currenciesService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());

		_context = new DataContext(*accountsRepository, *reportsRepository, *budgetsRepository, *goalsRepository, *alertsRepository,
			*schedulersRepository, *transactionsRepository, *tagsRepository, *currenciesRepository, *reportingRepository,
			*accountingService, *accountsService, *schedulersService, *budgetsService, *transactionsService, *tagsService,
			*reportsService, *alertsService, *goalsService, *reportingService, *currenciesService);
	}

	void SetUp() override {
		SetUpConnection();
		SetUpContext();
	}

	void TearDown() override {
		delete _context;
		delete _connection;
	}

protected:
	DataContext* _context;
	DataConnection* _connection;
};