#include "CurrenciesService.h"

CurrenciesService::CurrenciesService(CurrenciesRepository& currenciesRepository) :
	_currenciesRepository(currenciesRepository)
{
}

void CurrenciesService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

std::vector<std::shared_ptr<CurrencyPresentationModel>> CurrenciesService::GetAll() {
	auto currencies = _currenciesRepository.GetAll();

	std::vector<std::shared_ptr<CurrencyPresentationModel>> result;

	std::transform(currencies.begin(), currencies.end(), std::back_inserter(result), [&](const std::shared_ptr<CurrencyModel>& currency) {
		return std::make_shared<CurrencyPresentationModel>(*currency);
	});

	return result;
}

std::shared_ptr<CurrencyPresentationModel> CurrenciesService::GetById(int id) {
	auto currency = _currenciesRepository.GetById(id);

	if (currency) {
		auto model = std::make_shared<CurrencyPresentationModel>(*currency);

		return model;
	}

	return nullptr;
}

std::shared_ptr<CurrencyPresentationModel> CurrenciesService::GetBaseCurrency() {
	auto currency = _currenciesRepository.GetById(_baseCurrencyId);

	if (currency) {
		return std::make_shared<CurrencyPresentationModel>(*currency);
	}

	return nullptr;
}