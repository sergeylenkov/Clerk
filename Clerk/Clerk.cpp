#include "Clerk.h"
#include "Data/Models/Model.h"
#include "Data/DataContext.h"
#include "Utils/Icons.h"

IMPLEMENT_APP(ClerkApp)

bool ClerkApp::OnInit()
{
	wxString localePath(wxStandardPaths::Get().GetUserDataDir());
	localePath = localePath + "\\Resources\\Locales";

	int systemLang = wxLocale::GetSystemLanguage();

	_locale = new wxLocale();	
	_locale->Init(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);

	_locale->AddCatalogLookupPathPrefix(localePath);
	_locale->AddCatalog("locale");

	wxUILocale::UseDefault();

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
	ReportingRepository* reportingRepository = new ReportingRepository(*_connection);

	CurrenciesService* currenciesService = new CurrenciesService(*currenciesRepository);
	AccountsService* accountsService = new AccountsService(*accountsRepository, *currenciesService);
	AccountingService* accountingService = new AccountingService(*accountsService, *currenciesService);
	SchedulersService* schedulersService = new SchedulersService(*schedulersRepository, *accountsService, *currenciesService);
	BudgetsService* budgetsService = new BudgetsService(*budgetsRepository);
	TagsService* tagsService = new TagsService(*tagsRepository);
	TransactionsService* transactionsService = new TransactionsService(*transactionsRepository, *accountsService, *tagsService);	
	ReportsService* reportsService = new ReportsService(*reportsRepository);
	AlertsService* alertsService = new AlertsService(*alertsRepository);
	GoalsService* goalsService = new GoalsService(*goalsRepository);
	ReportingService* reportingService = new ReportingService(*reportingRepository, *currenciesService);

	currenciesService->SetBaseCurrency(Settings::GetInstance().GetBaseCurrencyId());

	_context = new DataContext(*accountsRepository, *reportsRepository, *budgetsRepository, *goalsRepository, *alertsRepository,
		*schedulersRepository, *transactionsRepository, *tagsRepository, *currenciesRepository, *reportingRepository,
		*accountingService, *accountsService, *schedulersService, *budgetsService, *transactionsService, *tagsService,
		*reportsService, *alertsService, *goalsService, *reportingService, *currenciesService);

	_icons = new Icons();

	MainWindow* frame = new MainWindow(*_context, *_icons);
	
	frame->Show(TRUE);
	frame->Center();	

	SetTopWindow(frame);

	return TRUE;
}

int ClerkApp::OnExit()
{	
	delete _connection;
	delete _context;	
	delete _icons;
	delete _locale;

	return 0;
}