#include "DataContext.h"

using namespace Clerk::Data;

DataContext::DataContext(
	AccountsRepository& accountsRepository,
	ReportsRepository& reportsRepository,
	BudgetsRepository& budgetsRepository,
	GoalsRepository& goalsRepository,
	AlertsRepository& alertsRepository,
	SchedulersRepository& schedulersRepository,
	TransactionsRepository& transactionsRepository,
	TagsRepository& tagsRepository,
	CurrenciesRepository& currenciesRepository,
	ExchangeRatesRepository& exchangeRatesRepository,
	ReportingRepository& reportingRepository,
	AccountingService& accountingService,
	AccountsService& accountsService,
	SchedulersService& schedulersService,
	BudgetsService& budgetsService,
	TransactionsService& transactionsService,
	TagsService& tagsService,
	ReportsService& reportsService,
	AlertsService& alertsService,
	GoalsService& goalsService,
	ReportingService& reportingService
):
	_accountsRepository(accountsRepository),
	_reportsRepository(reportsRepository),
	_budgetsRepository(budgetsRepository),
	_goalsRepository(goalsRepository),
	_alertsRepository(alertsRepository),
	_schedulersRepository(schedulersRepository),
	_transactionsRepository(transactionsRepository),
	_tagsRepository(tagsRepository),
	_currenciesRepository(currenciesRepository),
	_exchangeRatesRepository(exchangeRatesRepository),
	_reportingRepository(reportingRepository),
	_accountingService(accountingService),
	_accountsService(accountsService),
	_schedulersService(schedulersService),
	_budgetsService(budgetsService),
	_transactionsService(transactionsService),
	_tagsService(tagsService),
	_reportsService(reportsService),
	_alertsService(alertsService),
	_goalsService(goalsService),
	_reportingService(reportingService)
{	
}

DataContext::~DataContext() {
	delete& _accountsRepository;
	delete& _reportsRepository;
	delete& _budgetsRepository;
	delete& _goalsRepository;
	delete& _alertsRepository;
	delete& _schedulersRepository;
	delete& _transactionsRepository;
	delete& _tagsRepository;
	delete& _currenciesRepository;
	delete& _exchangeRatesRepository;
	delete& _reportingRepository;
	delete& _accountingService;
	delete& _accountsService;
	delete& _schedulersService;
	delete& _budgetsService;
	delete& _transactionsService;
	delete& _tagsService;
	delete& _reportsService;
	delete& _alertsService;
	delete& _goalsService;
	delete& _reportingService;
	delete _commandsInvoker;
}

AccountsRepository& DataContext::GetAccountsRepository() {
	return _accountsRepository;
}

ReportsRepository& DataContext::GetReportsRepository() {
	return _reportsRepository;
}

BudgetsRepository& DataContext::GetBudgetsRepository() {
	return _budgetsRepository;
}

GoalsRepository& DataContext::GetGoalsRepository() {
	return _goalsRepository;
}

AlertsRepository& DataContext::GetAlertsRepository() {
	return _alertsRepository;
}

SchedulersRepository& DataContext::GetSchedulersRepository() {
	return _schedulersRepository;
}

TransactionsRepository& DataContext::GetTransactionsRepository() {
	return _transactionsRepository;
}

TagsRepository& DataContext::GetTagsRepository() {
	return _tagsRepository;
}

CurrenciesRepository& DataContext::GetCurrenciesRepository() {
	return _currenciesRepository;
}

ExchangeRatesRepository& DataContext::GetExchangeRatesRepository() {
	return _exchangeRatesRepository;
}

ReportingRepository& DataContext::GetReportingRepository() {
	return _reportingRepository;
}
AccountingService& DataContext::GetAccountingService() {
	return _accountingService;
}

AccountsService& DataContext::GetAccountsService() {
	return _accountsService;
}

SchedulersService& DataContext::GetSchedulersService() {
	return _schedulersService;
}

BudgetsService& DataContext::GetBudgetsService() {
	return _budgetsService;
}

TransactionsService& DataContext::GetTransactionsService() {
	return _transactionsService;
}

TagsService& DataContext::GetTagsService() {
	return _tagsService;
}

ReportsService& DataContext::GetReportsService() {
	return _reportsService;
}

AlertsService& DataContext::GetAlertsService() {
	return _alertsService;
}

GoalsService& DataContext::GetGoalsService() {
	return _goalsService;
}

ReportingService& DataContext::GetReportingService() {
	return _reportingService;
}

void DataContext::SetCommandsInvoker(CommandsInvoker* commandsInvoker) {
	_commandsInvoker = commandsInvoker;
}

CommandsInvoker& DataContext::GetCommandsInvoker() {
	return *_commandsInvoker;
}