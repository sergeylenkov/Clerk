#include "AccountingService.h"

AccountingService::AccountingService(AccountsService& accountsService, CurrenciesService& currenciesService):
	_accountsService(accountsService),
	_currenciesService(currenciesService)
{
	_eventEmitter = new EventEmitter();
}

AccountingService::~AccountingService() {
	delete _eventEmitter;
}

void AccountingService::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

float AccountingService::GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate) {
	float result = 0;

	CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();

	for (auto& account : _accountsService.GetReceipts(fromDate, toDate)) {
		float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);

		result = result + (account->receipts * rate);
	}

	return result;
}

float AccountingService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	float result = 0;

	CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();

	for (auto& account : _accountsService.GetExpenses(fromDate, toDate)) {
		float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);

		result = result + (account->expenses * rate);
	}

	return result;
}

float AccountingService::GetBalance() {
	float result = 0;

	CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();

	for (auto& account : _accountsService.GetDeposits()) {
		float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);
		float balance = account->balance * rate;

		if (account->creditLimit > 0 && balance > 0) {			
			result = result + balance;
		}
		else {
			result = result + balance;
		}
	}

	return result;
}

float AccountingService::GetCredit() {
	float result = 0;

	CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();

	for (auto& account : _accountsService.GetByType(AccountType::Deposit)) {
		float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);
		float balance = account->balance * rate;

		if (account->creditLimit > 0 && balance > 0) {
			result = result + account->creditLimit;
		}
	}

	return result;
}