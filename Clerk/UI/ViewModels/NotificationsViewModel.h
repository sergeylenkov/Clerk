#pragma once

#include "../../Data/Services/AlertsService.h"

namespace Clerk {
	namespace UI {
		class NotificationsViewModel {
		public:
			NotificationsViewModel(AlertsService& alertsService);
			
			bool IsActive();

		private:
			AlertsService& _alertsService;
		};
	}
}