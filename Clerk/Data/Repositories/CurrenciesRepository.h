#pragma once

#include <vector>
#include <memory>
#include <map>
#include "BaseRepository.h"
#include "../Models/CurrencyModel.h"

namespace Clerk {
	namespace Data {
		class CurrenciesRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			CurrencyModel* GetById(int id);
			std::vector<CurrencyModel*> GetAll();			

		private:
			CurrencyModel* Load(int id);
			int _baseCurrencyId;
		};
	}
}
#pragma once
