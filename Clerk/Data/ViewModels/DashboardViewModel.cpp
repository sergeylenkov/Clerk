#include "DashboardViewModel.h"

using namespace Clerk::Data;

DashboardViewModel::DashboardViewModel(AccountingService& accountingService, TransactionsService& transactionsService, AccountsService& accountsService, Currency& currency) :
	_accountingService(accountingService),
	_transactionsService(transactionsService),
	_accountsService(accountsService),
	_currency(currency)
{
	_transactionsService.OnUpdate = [=]() {
		if (OnUpdate) {
			OnUpdate();
		}
	};
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

Currency& DashboardViewModel::GetCurrency() {
	return _currency;
}

std::vector<std::shared_ptr<AccountViewModel>> DashboardViewModel::GetAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Deposit);
	auto virtuals = _accountsService.GetByType(AccountType::Virtual);

	accounts.insert(accounts.end(), virtuals.begin(), virtuals.end());

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> DashboardViewModel::GetExpensesForMonth() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	auto expenses = _accountsService.GetExpenses(fromDate, toDate);

	std::sort(expenses.begin(), expenses.end(), [](auto a, auto b) {
		return a->balance > b->balance;
	});

	return expenses;
}

float DashboardViewModel::GetTotalExpensesForMonth() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetExpenses(fromDate, toDate);
}