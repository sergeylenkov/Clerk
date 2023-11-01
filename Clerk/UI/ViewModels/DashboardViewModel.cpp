#include "DashboardViewModel.h"

using namespace Clerk::UI;

DashboardViewModel::DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService,
	BudgetsService& budgetsService, SchedulersService& schedulersService, GoalsService& goalsService, CurrenciesService& currenciesService) :
	_accountingService(accountingService),
	_transactionsService(transactionsService),
	_accountsService(accountsService),
	_budgetsService(budgetsService),
	_schedulersService(schedulersService),
	_goalsService(goalsService),
	_currenciesService(currenciesService)
{
	_eventEmitter = new EventEmitter();

	unsigned int _subscriptionId = _transactionsService.Subscribe([&]() {
		_eventEmitter->Emit();
	});
}

DashboardViewModel::~DashboardViewModel() {
	_transactionsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void DashboardViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

float DashboardViewModel::GetTotalFunds() {
	return _accountingService.GetBalance() + _accountingService.GetCredit();
}

float DashboardViewModel::GetOwnFunds() {
	return _accountingService.GetBalance();
}

float DashboardViewModel::GetCreditFunds() {
	return _accountingService.GetCredit();
}

std::shared_ptr<CurrencyPresentationModel> DashboardViewModel::GetCurrency() {
	return _currenciesService.GetBaseCurrency();
}

std::vector<AccountPresentationModel*> DashboardViewModel::GetAccounts() {
	auto accounts = _accountsService.GetDepositsAndVirtuals();

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> DashboardViewModel::GetExpensesForMonth() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	auto expenses = _accountsService.GetExpenses(fromDate, toDate);

	std::sort(expenses.begin(), expenses.end(), [](auto a, auto b) {
		return a->expenses > b->expenses;
	});

	return expenses;
}

float DashboardViewModel::GetTotalReceiptsForMonth() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetReceipts(fromDate, toDate);
}

float DashboardViewModel::GetTotalExpensesForMonth() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetExpenses(fromDate, toDate);
}

std::vector<std::shared_ptr<BudgetPresentationModel>> DashboardViewModel::GetBudgets() {
	return _budgetsService.GetAll();
}

std::vector<std::shared_ptr<SchedulerPresentationModel>> DashboardViewModel::GetSchedulersForMonth() {
	wxDateTime today = wxDateTime::Now();
	wxDateTime month = wxDateTime::Now().Add(wxDateSpan(0, 0, 0, 30));

	return _schedulersService.GetByPeriod(today, month);
}

std::vector<AccountPresentationModel*> DashboardViewModel::GetDepts() {
	return _accountsService.GetDebts();
}

std::vector<std::shared_ptr<GoalPresentationModel>> DashboardViewModel::GetGoals() {
	return _goalsService.GetAll();
}