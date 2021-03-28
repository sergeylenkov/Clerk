#pragma once

#include <vector>
#include <memory>
#include <map>
#include "HashRepository.h"
#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		class CurrenciesRepository : HashRepository<Currency> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<Currency>> GetAll();
			std::shared_ptr<Currency> GetById(int id);
			void SetBaseCurrency(int id);
			std::shared_ptr<Currency> GetBaseCurrency();

		private:
			std::shared_ptr<Currency> Load(int id);
			int _baseCurrencyId;
		};
	}
}
#pragma once
