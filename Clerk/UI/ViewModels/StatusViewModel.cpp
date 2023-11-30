#include "StatusViewModel.h"

using namespace Clerk::UI;

StatusViewModel::StatusViewModel(AccountingService& accountingService, CurrenciesService& currenciesService, std::vector<int> selectedRates) :
	_accountingService(accountingService),
	_currenciesService(currenciesService)
{
	_selectedRates = selectedRates;
	_isExchangeRatesLoading = false;

	_eventEmitter = new EventEmitter();

	_accountingService.OnUpdate([&]() {
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
	auto baseCurrency = _currenciesService.GetBaseCurrency();

	wxString rates("");

	for (int id : _selectedRates) {
		auto currency = _currenciesService.GetById(id);
		float rate = _currenciesService.GetExchangeRate(*currency, *baseCurrency);

		rates = rates + wxNumberFormatter::ToString(rate, 2) + wxT(" ") + currency->sign + wxT("  ");
	}

	return rates.Trim();
}

std::shared_ptr<CurrencyPresentationModel> StatusViewModel::GetBaseCurrency() {
	return _currenciesService.GetBaseCurrency();
}

void StatusViewModel::SetIsExchangeRatesLoading(boolean isLoading) {
	_isExchangeRatesLoading = isLoading;
	_eventEmitter->Emit();
}

boolean StatusViewModel::GetIsExchangeRatesLoading() {
	return _isExchangeRatesLoading;
}