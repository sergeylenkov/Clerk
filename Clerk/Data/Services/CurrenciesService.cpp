#include "CurrenciesService.h"

CurrenciesService::CurrenciesService(CurrenciesRepository& currenciesRepository) :
	_currenciesRepository(currenciesRepository)
{
	_isLoading = false;
}

void CurrenciesService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

std::shared_ptr<CurrencyPresentationModel> CurrenciesService::GetById(int id) {
	auto currency = GetFromHash(id);

	if (currency) {
		return currency;
	}

	auto model = _currenciesRepository.GetById(id);

	if (model) {
		currency = std::make_shared<CurrencyPresentationModel>(*model);

		AddToHash(currency->id, currency);

		delete model;

		return currency;
	}

	return nullptr;
}

shared_vector<CurrencyPresentationModel> CurrenciesService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _currenciesRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto currency = std::make_shared<CurrencyPresentationModel>(*model);
			AddToHash(currency->id, currency);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}


std::shared_ptr<CurrencyPresentationModel> CurrenciesService::GetBaseCurrency() {
	return GetById(_baseCurrencyId);
}

float CurrenciesService::GetExchangeRate(const CurrencyPresentationModel& fromCurrency, const CurrencyPresentationModel& toCurrency) {
	if (_exchangeRates.empty()) {
		_exchangeRates = _currenciesRepository.LoadExchangeRates();
	}

	auto pair = std::make_pair(fromCurrency.id, toCurrency.id);

	auto search = _exchangeRates.find(pair);

	if (search != _exchangeRates.end()) {
		return search->second;
	}

	return 1;
}

void CurrenciesService::UpdatedExchangeRates() {
	_exchangeRates.clear();

	CBRRatesLoader* loader = new CBRRatesLoader(_currenciesRepository.GetConnection());
	loader->Load();

	delete loader;
}