#include "NotificationsViewModel.h"

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService) :
	_alertsService(alertsService)
{
	_alerts = _alertsService.GetActive();
}

shared_vector<AlertPresentationModel> NotificationsViewModel::GetActiveAlerts() {
	return _alerts;
}

bool NotificationsViewModel::IsActive() {
	return _alerts.size() > 0;
}