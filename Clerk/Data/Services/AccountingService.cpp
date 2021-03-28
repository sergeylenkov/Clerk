#include "AccountingService.h"

AccountingService::AccountingService(AccountsRepository& accountsRepository, ExchangeRatesRepository& exchangeRatesRepository):
	_accountsRepository(accountsRepository),
	_exchangeRatesRepository(exchangeRatesRepository)
{
}

void AccountingService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

float AccountingService::GetReceipts(wxDateTime& fromDate, wxDateTime& toDate) {
	float result = 0;

	for (auto& account : _accountsRepository.GetByType(AccountType::Receipt)) {
		float amount = _accountsRepository.GetReceipts(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}

	return result;
}

float AccountingService::GetExpenses(wxDateTime& fromDate, wxDateTime& toDate) {
	float result = 0;

	for (auto& account : _accountsRepository.GetByType(AccountType::Expens)) {
		float amount = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}

	for (auto& account : _accountsRepository.GetByType(AccountType::Debt)) {
		float amount = _accountsRepository.GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);

		result = result + (amount * rate);
	}

	return result;
}

float AccountingService::GetBalance() {
	float result = 0;

	for (auto& account : _accountsRepository.GetByType(AccountType::Deposit)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);
		float balance = _accountsRepository.GetBalance(account->id, account->type) * rate;

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

	for (auto& account : _accountsRepository.GetByType(AccountType::Deposit)) {
		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);
		float balance = _accountsRepository.GetBalance(account->id, account->type) * rate;

		if (account->creditLimit > 0 && balance > 0) {
			result = result + account->creditLimit;
		}
	}

	return result;
}