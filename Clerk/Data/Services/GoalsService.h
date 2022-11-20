#pragma once

#include "../Repositories/GoalsRepository.h"
#include "../../UI/PresentationModels/GoalPresentationModel.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class GoalsService {
		public:
			GoalsService(GoalsRepository& goalsRepository);

			std::shared_ptr<GoalPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<GoalPresentationModel>> GetAll();

		private:
			GoalsRepository& _goalsRepository;
		};
	}
}

