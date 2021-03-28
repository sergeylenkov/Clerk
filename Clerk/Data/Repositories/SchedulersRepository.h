#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "HashRepository.h"
#include "../Models/SchedulerModel.h"

namespace Clerk {
	namespace Data {
		class SchedulersRepository : HashRepository<SchedulerModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<SchedulerModel>> GetAll();
			std::shared_ptr<SchedulerModel> GetById(int id);
			void Save(SchedulerModel& scheduler);
			void Delete(const SchedulerModel& scheduler);

		private:
			std::shared_ptr<SchedulerModel> Load(int id);
		};
	}
}
