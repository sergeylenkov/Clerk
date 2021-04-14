#include "DashboardViewModel.h"

using namespace Clerk::Data;

DashboardViewModel::DashboardViewModel(AccountingService& accountingService, Currency& currency) :
	_accountingService(accountingService),
	_currency(currency)
{

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