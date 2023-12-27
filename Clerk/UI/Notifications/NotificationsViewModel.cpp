#include "NotificationsViewModel.h"

using namespace Clerk::UI;

NotificationsViewModel::NotificationsViewModel(AlertsService& alertsService, TransactionsService& transactionsService, SchedulersService& schedulersService):
	_alertsService(alertsService),
	_transactionsService(transactionsService),
	_schedulersService(schedulersService)
{
	UpdateNotifications();

	_eventEmitter = new EventEmitter();

	_subscriptionId = _transactionsService.Subscribe([&]() {
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

shared_vector<SchedulerPresentationModel> NotificationsViewModel::GetActiveSchedulers() {
	return _activeSchedulers;
}

bool NotificationsViewModel::IsActive() {
	return _activeAlerts.size() > 0 || _activeSchedulers.size() > 0;
}

void NotificationsViewModel::DismissAlert(AlertPresentationModel& alert) {
	_dismissiedAlerts.insert(alert.id);

	UpdateNotifications();

	_eventEmitter->Emit();
}

void NotificationsViewModel::DismissScheduler(SchedulerPresentationModel& scheduler) {
	_dismissiedSchedulers.insert(scheduler.id);

	UpdateNotifications();

	_eventEmitter->Emit();
}

void NotificationsViewModel::ExecScheduler(SchedulerPresentationModel& scheduler) {
	_schedulersService.Execute(scheduler);

	_dismissiedSchedulers.insert(scheduler.id);

	UpdateNotifications();

	_eventEmitter->Emit();
}

void NotificationsViewModel::SkipScheduler(SchedulerPresentationModel& scheduler) {
	_schedulersService.Skip(scheduler);

	_dismissiedSchedulers.insert(scheduler.id);

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

	_activeSchedulers.clear();

	wxDateTime date = wxDateTime::Today();
	date.Add(wxTimeSpan::Day());
	date.ResetTime();

	for (auto& scheduler : _schedulersService.GetActiveOnDate(date)) {
		if (_dismissiedSchedulers.count(scheduler->id) == 0) {
			_activeSchedulers.push_back(scheduler);
		}
	}
}