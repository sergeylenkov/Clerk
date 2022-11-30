#pragma once

#include <vector>
#include <memory>
#include <map>
#include "HashRepository.h"
#include "../Models/GoalModel.h"

namespace Clerk {
	namespace Data {
		class GoalsRepository : HashRepository<GoalModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<GoalModel>> GetAll();
			std::shared_ptr<GoalModel> GetById(int id);
			float GetBalance(std::wstring& accountIds);

			void Save(GoalModel& goal);
			void Delete(const GoalModel& goal);

		private:
			std::shared_ptr<GoalModel> Load(int id);
		};
	}
}