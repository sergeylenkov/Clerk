#pragma once

#include "../Repositories/BudgetsRepository.h"
#include "../../UI/PresentationModels/BudgetPresentationModel.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class BudgetsService {
		public:
			BudgetsService(BudgetsRepository& budgetsRepository);

			std::shared_ptr<BudgetPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<BudgetPresentationModel>> GetAll();

		private:
			BudgetsRepository& _budgetsRepository;
		};
	}
}

