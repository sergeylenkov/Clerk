#pragma once

#include <vector>
#include <memory>
#include <map>
#include "BaseRepository.h"
#include "../Models/CurrencyModel.h"
#include "../../Utils/Types.h"

namespace Clerk {
	namespace Data {
		class CurrenciesRepository : public BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			CurrencyModel* GetById(int id);
			std::vector<CurrencyModel*> GetAll();
			exhange_rate_map LoadExchangeRates();

		private:
			CurrencyModel* Load(int id);

			int _baseCurrencyId;			
		};
	}
}
#pragma once
