#pragma once

#include "../Repositories/AlertsRepository.h"
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
			~AlertsService();

			std::shared_ptr<AlertPresentationModel> GetById(int id);
			shared_vector<AlertPresentationModel> GetAll();

		private:
			AlertsRepository& _alertsRepository;

			boolean _isLoading;
		};
	}
}

