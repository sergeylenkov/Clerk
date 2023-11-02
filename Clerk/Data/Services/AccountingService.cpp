#include "AccountingService.h"

AccountingService::AccountingService(AccountsService& accountsService, ExchangeRatesRepository& exchangeRatesRepository):
	_accountsService(accountsService),
	_exchangeRatesRepository(exchangeRatesRepository)
{
	_eventEmitter = new EventEmitter();
}

AccountingService::~AccountingService() {
	delete _eventEmitter;
}

void AccountingService::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

void AccountingService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

float AccountingService::GetReceipts(const wxDateTime& fromDate, const wxDateTime& toDate) {
	float result = 0;

	for (auto& account : _accountsService.GetReceipts(fromDate, toDate)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currency->id, _baseCurrencyId);

		result = result + (account->receipts * rate);
	}

	return result;
}

float AccountingService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	float result = 0;

	for (auto& account : _accountsService.GetExpenses(fromDate, toDate)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currency->id, _baseCurrencyId);

		result = result + (account->expenses * rate);
	}

	return result;
}

float AccountingService::GetBalance() {
	float result = 0;

	for (auto& account : _accountsService.GetDeposits()) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currency->id, _baseCurrencyId);
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

	for (auto& account : _accountsService.GetByType(AccountType::Deposit)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currency->id, _baseCurrencyId);
		float balance = account->balance * rate;

		if (account->creditLimit > 0 && balance > 0) {
			result = result + account->creditLimit;
		}
	}

	return result;
}