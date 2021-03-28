#pragma once

#include "../Repositories/GoalsRepository.h"
#include "../ViewModels/GoalViewModel.h"

namespace Clerk {
	namespace Data {
		class GoalsService {
		public:
			GoalsService(GoalsRepository& goalsRepository);

			std::shared_ptr<GoalViewModel> GetById(int id);
			std::vector<std::shared_ptr<GoalViewModel>> GetAll();

		private:
			GoalsRepository& _goalsRepository;
		};
	}
}

