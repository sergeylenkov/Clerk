#include "StatusbarViewModel.h"

using namespace Clerk::UI;

StatusbarViewModel::StatusbarViewModel(AccountingService& accountingService, CurrenciesService& currenciesService, std::vector<int> selectedRates) :
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

StatusbarViewModel::~StatusbarViewModel() {
	delete _eventEmitter;
}

void StatusbarViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

float StatusbarViewModel::GetBalance() {
	return _accountingService.GetBalance();
}

float StatusbarViewModel::GetReceipts() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetReceipts(fromDate, toDate);
}

float StatusbarViewModel::GetExpenses() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	return _accountingService.GetExpenses(fromDate, toDate);
}

wxString StatusbarViewModel::GetExchangeRates() {
	auto baseCurrency = _currenciesService.GetBaseCurrency();

	wxString rates("");

	for (int id : _selectedRates) {
		auto currency = _currenciesService.GetById(id);
		float rate = _currenciesService.GetExchangeRate(*currency, *baseCurrency);

		rates = rates + Format::Amount(rate) + wxT(" ") + currency->sign + wxT("  ");
	}

	return rates.Trim();
}

std::shared_ptr<CurrencyPresentationModel> StatusbarViewModel::GetBaseCurrency() {
	return _currenciesService.GetBaseCurrency();
}

void StatusbarViewModel::SetIsExchangeRatesLoading(boolean isLoading) {
	_isExchangeRatesLoading = isLoading;
	_eventEmitter->Emit();
}

boolean StatusbarViewModel::GetIsExchangeRatesLoading() {
	return _isExchangeRatesLoading;
}