#pragma once

#include "../Repositories/AlertsRepository.h"
#include "AccountsService.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"
#include "./HashService.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class AlertsService : HashService<std::shared_ptr<AlertPresentationModel>> {
		public:
			AlertsService(AlertsRepository& alertsRepository);

			std::shared_ptr<AlertPresentationModel> GetById(int id);
			shared_vector<AlertPresentationModel> GetAll();
			shared_vector<AlertPresentationModel> GetActive();
			void Reload();

		private:
			AlertsRepository& _alertsRepository;
		};
	}
}

