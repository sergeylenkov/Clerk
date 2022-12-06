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

	/*for (auto& account : _accountsService.GetByType(AccountType::Receipt)) {
		float amount = _accountsService.GetReceipts(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}*/

	return result;
}

float AccountingService::GetExpenses(const wxDateTime& fromDate, const wxDateTime& toDate) {
	float result = 0;

	/*for (auto& account : _accountsService.GetByType(AccountType::Expens)) {
		float amount = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}

	for (auto& account : _accountsService.GetByType(AccountType::Debt)) {
		float amount = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}*/

	return result;
}

float AccountingService::GetBalance() {
	float result = 0;

	/*for (auto& account : _accountsRepository.GetByType(AccountType::Deposit)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);
		float balance = _accountsRepository.GetBalance(account->id, account->type) * rate;

		if (account->creditLimit > 0 && balance > 0) {			
			result = result + balance;
		}
		else {
			result = result + balance;
		}
	}*/

	return result;
}

float AccountingService::GetCredit() {
	float result = 0;

	/*for (auto& account : _accountsRepository.GetByType(AccountType::Deposit)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);
		float balance = _accountsRepository.GetBalance(account->id, account->type) * rate;

		if (account->creditLimit > 0 && balance > 0) {
			result = result + account->creditLimit;
		}
	}*/

	return result;
}