#pragma once

#include <vector>
#include <memory>
#include "BaseRepository.h"
#include "../Models/AlertModel.h"

namespace Clerk {
	namespace Data {
		class AlertsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			AlertModel* GetById(int id);
			std::vector<AlertModel*> GetAll();			
			float GetBalance(const AlertModel& alert);
			int Save(const AlertModel& alert);
			void Delete(const AlertModel& alert);

		private:
			AlertModel* Load(int id);			
		};
	}
}
