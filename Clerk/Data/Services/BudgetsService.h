#pragma once

#include "../Repositories/BudgetsRepository.h"
#include "../ViewModels/BudgetViewModel.h"

namespace Clerk {
	namespace Data {
		class BudgetsService {
		public:
			BudgetsService(BudgetsRepository& budgetsRepository);

			std::shared_ptr<BudgetViewModel> GetById(int id);
			std::vector<std::shared_ptr<BudgetViewModel>> GetAll();

		private:
			BudgetsRepository& _budgetsRepository;
		};
	}
}

