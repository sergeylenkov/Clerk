#include "NotificationsViewModel.h"

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService) :
	_alertsService(alertsService)
{
	_activeAlerts = _alertsService.GetActive();
}

shared_vector<AlertPresentationModel> NotificationsViewModel::GetActiveAlerts() {
	return _activeAlerts;
}

bool NotificationsViewModel::IsActive() {
	return _activeAlerts.size() > 0;
}

void NotificationsViewModel::Dismiss(AlertPresentationModel& alert) {
	alert.isDismissed = true;

	_activeAlerts = _alertsService.GetActive();

	if (OnUpdate) {
		OnUpdate();
	}
}