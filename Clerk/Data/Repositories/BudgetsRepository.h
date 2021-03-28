#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include "HashRepository.h"
#include "../Models/BudgetModel.h"

namespace Clerk {
	namespace Data {
		class BudgetsRepository : HashRepository<BudgetModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<BudgetModel>> GetAll();
			std::shared_ptr<BudgetModel> GetById(int id);
			float GetExpenses(const BudgetModel& budget, std::string& fromDate, std::string& toDate);
			void Save(BudgetModel& budget);
			void Delete(const BudgetModel& budget);

		private:
			std::shared_ptr<BudgetModel> Load(int id);
		};
	}
}
