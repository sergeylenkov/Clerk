#pragma once

#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"
#include "../Repositories/GoalsRepository.h"
#include "../PresentationModels/GoalPresentationModel.h"
#include "HashService.h"

namespace Clerk {
	namespace Data {
		class GoalsService : HashService<std::shared_ptr<GoalPresentationModel>> {
		public:
			GoalsService(GoalsRepository& goalsRepository);
			~GoalsService();

			std::shared_ptr<GoalPresentationModel> GetById(int id);
			shared_vector<GoalPresentationModel> GetAll();

			std::shared_ptr<GoalPresentationModel> Save(GoalPresentationModel& budget);
			void Delete(GoalPresentationModel& budget);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			GoalsRepository& _goalsRepository;
			EventEmitter* _eventEmitter;

			float GetBalance(GoalPresentationModel& goal);
		};
	}
}

