#pragma once

#include "../Repositories/CurrenciesRepository.h"
#include "../../UI/PresentationModels/CurrencyPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class CurrenciesService {
		public:
			CurrenciesService(CurrenciesRepository& currenciesRepository);

			void SetBaseCurrency(int id);
			std::vector<std::shared_ptr<CurrencyPresentationModel>> GetAll();
			std::shared_ptr<CurrencyPresentationModel> GetById(int id);
			std::shared_ptr<CurrencyPresentationModel> GetBaseCurrency();

		private:
			CurrenciesRepository& _currenciesRepository;
			int _baseCurrencyId = 0;
		};
	}
}
