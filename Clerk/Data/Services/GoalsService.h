#pragma once

#include "../Repositories/GoalsRepository.h"
#include "../../UI/PresentationModels/GoalPresentationModel.h"
#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"
#include "HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class GoalsService : HashService<std::shared_ptr<GoalPresentationModel>> {
		public:
			GoalsService(GoalsRepository& goalsRepository);
			~GoalsService();

			std::shared_ptr<GoalPresentationModel> GetById(int id);
			shared_vector<GoalPresentationModel> GetAll();

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			GoalsRepository& _goalsRepository;
			EventEmitter* _eventEmitter;

			float GetBalance(GoalPresentationModel& goal);
		};
	}
}

