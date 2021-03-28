#pragma once

#include <vector>
#include <memory>
#include "HashRepository.h"
#include "../Models/AlertModel.h"

namespace Clerk {
	namespace Data {
		class AlertsRepository : HashRepository<AlertModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<AlertModel>> GetAll();
			std::shared_ptr<AlertModel> GetById(int id);
			float GetBalance(const AlertModel& alert);
			void Save(AlertModel& alert);
			void Delete(const AlertModel& alert);

		private:
			std::shared_ptr<AlertModel> Load(int id);			
		};
	}
}
