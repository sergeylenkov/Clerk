#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include "BaseRepository.h"
#include "../Models/SchedulerModel.h"

namespace Clerk {
	namespace Data {
		class SchedulersRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			SchedulerModel* GetById(int id);
			std::vector<SchedulerModel*> GetAll();			
			int Save(const SchedulerModel& scheduler);
			void Delete(const SchedulerModel& scheduler);

		private:
			SchedulerModel* Load(int id);
		};
	}
}
