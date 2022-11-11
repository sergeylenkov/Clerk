#include "StatusViewModel.h"

using namespace Clerk::Data;

StatusViewModel::StatusViewModel(AccountingService& accountingService, ExchangeRatesRepository& exchangeRatesRepository, CurrenciesRepository& currenciesRepository, std::vector<int> selectedRates) :
	_accountingService(accountingService),
	_exchangeRatesRepository(exchangeRatesRepository),
	_currenciesRepository(currenciesRepository)
{
	_selectedRates = selectedRates;

	_eventEmitter = new EventEmitter();

	_accountingService.OnUpdate([=]() {
		_eventEmitter->Emit();
	});
}

StatusViewModel::~StatusViewModel() {
	delete _eventEmitter;
}

void StatusViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

float StatusViewModel::GetBalance() {
	return _accountingService.GetBalance();
}

float StatusViewModel::GetReceipts() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetReceipts(fromDate, toDate);
}

float StatusViewModel::GetExpenses() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetExpenses(fromDate, toDate);
}

wxString StatusViewModel::GetExchangeRates() {
	auto baseCurrency = _currenciesRepository.GetBaseCurrency();

	wxString rates("");

	for (int id : _selectedRates) {
		float rate = _exchangeRatesRepository.GetExchangeRate(id, baseCurrency->id);
		auto currency = _currenciesRepository.GetById(id);

		rates = rates + wxNumberFormatter::ToString(rate, 2) + wxT(" ") + *currency->sign + wxT("  ");
	}

	return rates.Trim();
}

std::shared_ptr<Currency> StatusViewModel::GetBaseCurrency() {
	return _currenciesRepository.GetBaseCurrency();
}