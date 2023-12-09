#pragma once

#include "../../Data/Services/AlertsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"
#include "../../Utils/EventEmitter.h"
#include "../../Utils/Types.h"

namespace Clerk {
	namespace UI {
		class NotificationsViewModel {
		public:
			NotificationsViewModel(AlertsService& alertsService, TransactionsService& transactionsService);
			~NotificationsViewModel();

			shared_vector<AlertPresentationModel> GetActiveAlerts();
			bool IsActive();
			void Dismiss(AlertPresentationModel& alert);

			void OnUpdate(std::function<void()> fn);

		private:
			AlertsService& _alertsService;
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;

			shared_vector<AlertPresentationModel> _activeAlerts;
		};
	}
}