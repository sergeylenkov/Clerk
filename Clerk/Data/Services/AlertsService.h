#pragma once

#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"
#include "../Repositories/AlertsRepository.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "AccountsService.h"
#include "HashService.h"

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

