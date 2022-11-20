#include "StatusViewModel.h"

using namespace Clerk::UI;

StatusViewModel::StatusViewModel(AccountingService& accountingService, ExchangeRatesRepository& exchangeRatesRepository, CurrenciesService& currenciesService, std::vector<int> selectedRates) :
	_accountingService(accountingService),
	_exchangeRatesRepository(exchangeRatesRepository),
	_currenciesService(currenciesService)
{
	_selectedRates = selectedRates;

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
		float rate = _exchangeRatesRepository.GetExchangeRate(id, baseCurrency->id);
		auto currency = _currenciesService.GetById(id);

		rates = rates + wxNumberFormatter::ToString(rate, 2) + wxT(" ") + currency->sign + wxT("  ");
	}

	return rates.Trim();
}

std::shared_ptr<CurrencyPresentationModel> StatusViewModel::GetBaseCurrency() {
	return _currenciesService.GetBaseCurrency();
}