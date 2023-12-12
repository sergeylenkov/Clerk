#pragma once

#include "../Repositories/AlertsRepository.h"
#include "AccountsService.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"
#include "./HashService.h"
#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"

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
			shared_vector<AlertPresentationModel> GetActive();
			void Reload();

			std::shared_ptr<AlertPresentationModel> Save(AlertPresentationModel& alert);
			void Delete(AlertPresentationModel& alert);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			AlertsRepository& _alertsRepository;
			EventEmitter* _eventEmitter;
		};
	}
}

