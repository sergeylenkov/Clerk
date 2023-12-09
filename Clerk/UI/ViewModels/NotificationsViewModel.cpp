#include "NotificationsViewModel.h"

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService, TransactionsService& transactionsService):
	_alertsService(alertsService),
	_transactionsService(transactionsService)
{
	_activeAlerts = _alertsService.GetActive();

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
	alert.isDismissed = true;

	_activeAlerts = _alertsService.GetActive();

	_eventEmitter->Emit();
}