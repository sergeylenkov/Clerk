#pragma once

#include "../Repositories/BudgetsRepository.h"
#include "../../UI/PresentationModels/BudgetPresentationModel.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class BudgetsService : HashService<std::shared_ptr<BudgetPresentationModel>> {
		public:
			BudgetsService(BudgetsRepository& budgetsRepository);

			std::shared_ptr<BudgetPresentationModel> GetById(int id);
			shared_vector<BudgetPresentationModel> GetAll();
			void UpdateBalance();			

		private:
			BudgetsRepository& _budgetsRepository;

			float GetExpenses(BudgetPresentationModel& budget, std::string& fromDate, std::string& toDate);
		};
	}
}

