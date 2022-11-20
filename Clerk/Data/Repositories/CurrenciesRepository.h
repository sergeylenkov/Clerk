#pragma once

#include <vector>
#include <memory>
#include <map>
#include "HashRepository.h"
#include "../Models/CurrencyModel.h"

namespace Clerk {
	namespace Data {
		class CurrenciesRepository : HashRepository<CurrencyModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<CurrencyModel>> GetAll();
			std::shared_ptr<CurrencyModel> GetById(int id);
			void SetBaseCurrency(int id);
			std::shared_ptr<CurrencyModel> GetBaseCurrency();

		private:
			std::shared_ptr<CurrencyModel> Load(int id);
			int _baseCurrencyId;
		};
	}
}
#pragma once
