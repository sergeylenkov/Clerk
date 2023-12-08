#include "NotificationsViewModel.h"

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService) :
	_alertsService(alertsService) {
}

bool NotificationsViewModel::IsActive() {
	return true;
}