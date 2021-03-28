#pragma once

#include <memory>
#include <map>
#include "BaseRepository.h"

namespace Clerk {
	namespace Data {
		class ExchangeRatesRepository : BaseRepository {
		public:
			BaseRepository::BaseRepository;

			float GetExchangeRate(int fromCurrencyId, int toCurrencyId);
			
		private:
			std::map<std::pair<int, int>, float> _hash;

			void Load();
		};
	}
}
