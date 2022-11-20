#include "Clerk.h"
#include "Data/Models/Model.h"
#include "Data/DataContext.h"
#include "Utils/Icons.h"

IMPLEMENT_APP(ClerkApp)

bool ClerkApp::OnInit()
{
	_locale = new wxLocale();
	_locale->Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT);

	Settings::GetInstance().Open("Config.json");

	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), "Database.sqlite");
	std::string dbPath(path.GetFullPath().char_str());

	_connection = new DataConnection(std::move(dbPath));

	AccountsRepository* accountsRepository = new AccountsRepository(*_connection);
	ReportsRepository* reportsRepository = new ReportsRepository(*_connection);
	BudgetsRepository* budgetsRepository = new BudgetsRepository(*_connection);
	GoalsRepository* goalsRepository = new GoalsRepository(*_connection);
	AlertsRepository* alertsRepository = new AlertsRepository(*_connection);
	SchedulersRepository* schedulersRepository = new SchedulersRepository(*_connection);
	TransactionsRepository* transactionsRepository = new TransactionsRepository(*_connection);
	TagsRepository* tagsRepository = new TagsRepository(*_connection);
	CurrenciesRepository* currenciesRepository = new CurrenciesRepository(*_connection);
	ExchangeRatesRepository* exchangeRatesRepository = new ExchangeRatesRepository(*_connection);
	ReportingRepository* reportingRepository = new ReportingRepository(*_connection);

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

	_context = new DataContext(*accountsRepository, *reportsRepository, *budgetsRepository, *goalsRepository, *alertsRepository,
		*schedulersRepository, *transactionsRepository, *tagsRepository, *currenciesRepository, *exchangeRatesRepository, *reportingRepository,
		*accountingService, *accountsService, *schedulersService, *budgetsService, *transactionsService, *tagsService,
		*reportsService, *alertsService, *goalsService, *reportingService, *currenciesService);

	_icons = new Icons();

	MainWindow *frame = new MainWindow(*_context, *_icons);
	frame->Show(TRUE);
	frame->Center();

	SetTopWindow(frame);

	return TRUE;
}

int ClerkApp::OnExit()
{	
	delete _context;
	delete _connection;
	delete _icons;
	delete _locale;

	return 0;
}