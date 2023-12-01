#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include "BaseRepository.h"
#include "../Models/BudgetModel.h"

namespace Clerk {
	namespace Data {
		class BudgetsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			BudgetModel* GetById(int id);
			std::vector<BudgetModel*> GetAll();			
			float GetExpenses(std::string accountsIds, std::string& fromDate, std::string& toDate);
			int Save(const BudgetModel& budget);
			void Delete(const BudgetModel& budget);

		private:
			BudgetModel* Load(int id);
		};
	}
}
