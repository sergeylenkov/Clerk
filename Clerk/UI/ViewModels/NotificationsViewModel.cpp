#include "NotificationsViewModel.h"

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService, TransactionsService& transactionsService):
	_alertsService(alertsService),
	_transactionsService(transactionsService)
{
	UpdateNotifications();

	_eventEmitter = new EventEmitter();

	unsigned int _subscriptionId = _transactionsService.Subscribe([&]() {
		_alertsService.Reload();
		_eventEmitter->Emit();
	});
}

NotificationsViewModel::~NotificationsViewModel() {
	_transactionsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void NotificationsViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

shared_vector<AlertPresentationModel> NotificationsViewModel::GetActiveAlerts() {
	return _activeAlerts;
}

bool NotificationsViewModel::IsActive() {
	return _activeAlerts.size() > 0;
}

void NotificationsViewModel::Dismiss(AlertPresentationModel& alert) {
	_dismissiedAlerts.insert(alert.id);

	UpdateNotifications();

	_eventEmitter->Emit();
}

void NotificationsViewModel::UpdateNotifications() {
	_activeAlerts.clear();

	for (auto& alert : _alertsService.GetActive()) {
		if (_dismissiedAlerts.count(alert->id) == 0) {
			_activeAlerts.push_back(alert);
		}
	}
}