#pragma once

#include "../Repositories/CurrenciesRepository.h"
#include "../../UI/PresentationModels/CurrencyPresentationModel.h"
#include "../../Utils/Types.h"
#include "../ExchangeRates/CBRRatesLoader.h"
#include "HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class CurrenciesService : HashService<std::shared_ptr<CurrencyPresentationModel>> {
		public:
			CurrenciesService(CurrenciesRepository& currenciesRepository);

			void SetBaseCurrency(int id);
			shared_vector<CurrencyPresentationModel> GetAll();
			std::shared_ptr<CurrencyPresentationModel> GetById(int id);
			std::shared_ptr<CurrencyPresentationModel> GetBaseCurrency();
			float GetExchangeRate(const CurrencyPresentationModel& fromCurrency, const CurrencyPresentationModel& toCurrency);
			void UpdatedExchangeRates();

		private:
			CurrenciesRepository& _currenciesRepository;

			int _baseCurrencyId = 0;
			exhange_rate_map _exchangeRates;
		};
	}
}

