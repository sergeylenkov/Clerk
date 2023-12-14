#pragma once

#include <set>
#include "../../Data/Services/AlertsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Data/Services/SchedulersService.h"
#include "../../UI/PresentationModels/AlertPresentationModel.h"
#include "../../UI/PresentationModels/SchedulerPresentationModel.h"
#include "../../Utils/EventEmitter.h"
#include "../../Utils/Types.h"

namespace Clerk {
	namespace UI {
		class NotificationsViewModel {
		public:
			NotificationsViewModel(AlertsService& alertsService, TransactionsService& transactionsService, SchedulersService& schedulersService);
			~NotificationsViewModel();

			shared_vector<AlertPresentationModel> GetActiveAlerts();
			shared_vector<SchedulerPresentationModel> GetActiveSchedulers();
			bool IsActive();
			void DismissAlert(AlertPresentationModel& alert);
			void DismissScheduler(SchedulerPresentationModel& scheduler);
			void ExecScheduler(SchedulerPresentationModel& scheduler);
			void SkipScheduler(SchedulerPresentationModel& scheduler);

			void OnUpdate(std::function<void()> fn);

		private:
			AlertsService& _alertsService;
			TransactionsService& _transactionsService;
			SchedulersService& _schedulersService;
			EventEmitter* _eventEmitter;
			unsigned int _subscriptionId;

			shared_vector<AlertPresentationModel> _activeAlerts;
			shared_vector< SchedulerPresentationModel> _activeSchedulers;
			std::set<int> _dismissiedAlerts;
			std::set<int> _dismissiedSchedulers;

			void UpdateNotifications();
		};
	}
}