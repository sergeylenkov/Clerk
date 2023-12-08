#pragma once

#include "../../Data/Services/AlertsService.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"
#include "../../Utils/Types.h"

namespace Clerk {
	namespace UI {
		class NotificationsViewModel {
		public:
			NotificationsViewModel(AlertsService& alertsService);
			
			shared_vector<AlertPresentationModel> GetActiveAlerts();
			bool IsActive();

		private:
			AlertsService& _alertsService;

			shared_vector<AlertPresentationModel> _alerts;
		};
	}
}