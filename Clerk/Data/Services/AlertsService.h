#pragma once

#include "../Repositories/AlertsRepository.h"
#include "../ViewModels/AlertViewModel.h"

namespace Clerk {
	namespace Data {
		class AlertsService {
		public:
			AlertsService(AlertsRepository& alertsRepository);

			std::shared_ptr<AlertViewModel> GetById(int id);
			std::vector<std::shared_ptr<AlertViewModel>> GetAll();

		private:
			AlertsRepository& _alertsRepository;
		};
	}
}

