#pragma once

#include "../Repositories/AlertsRepository.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class AlertsService {
		public:
			AlertsService(AlertsRepository& alertsRepository);

			std::shared_ptr<AlertPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<AlertPresentationModel>> GetAll();

		private:
			AlertsRepository& _alertsRepository;
		};
	}
}

