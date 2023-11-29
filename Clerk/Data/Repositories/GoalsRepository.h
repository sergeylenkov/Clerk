#pragma once

#include <vector>
#include <memory>
#include <map>
#include <numeric>
#include "BaseRepository.h"
#include "../Models/GoalModel.h"

namespace Clerk {
	namespace Data {
		class GoalsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			GoalModel* GetById(int id);
			std::vector<GoalModel*> GetAll();			
			float GetBalance(std::vector<int> accountIds);

			int Save(const GoalModel& goal);
			void Delete(const GoalModel& goal);

		private:
			GoalModel* Load(int id);
		};
	}
}