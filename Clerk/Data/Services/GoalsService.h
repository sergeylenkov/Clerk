#pragma once

#include "../Repositories/GoalsRepository.h"
#include "../../UI/PresentationModels/GoalPresentationModel.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class GoalsService : HashService<std::shared_ptr<GoalPresentationModel>> {
		public:
			GoalsService(GoalsRepository& goalsRepository);

			std::shared_ptr<GoalPresentationModel> GetById(int id);
			shared_vector<GoalPresentationModel> GetAll();

		private:
			GoalsRepository& _goalsRepository;
		};
	}
}

