#include "AlertsService.h"

AlertsService::AlertsService(AlertsRepository& alertsRepository) :
	_alertsRepository(alertsRepository)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

AlertsService::~AlertsService() {
	delete _eventEmitter;
}

unsigned int AlertsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void AlertsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

std::shared_ptr<AlertPresentationModel> AlertsService::GetById(int id) {
	auto alert = GetFromHash(id);

	if (alert) {
		return alert;
	}

	auto model = _alertsRepository.GetById(id);

	if (model) {
		alert = std::make_shared<AlertPresentationModel>(*model);
		alert->balance = _alertsRepository.GetBalance(model->accountIds);

		AddToHash(alert->id, alert);

		delete model;

		return alert;
	}

	return nullptr;
}

shared_vector<AlertPresentationModel> AlertsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _alertsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto alert = std::make_shared<AlertPresentationModel>(*model);
			alert->balance = _alertsRepository.GetBalance(model->accountIds);

			AddToHash(alert->id, alert);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}

shared_vector<AlertPresentationModel> AlertsService::GetActive() {
	shared_vector<AlertPresentationModel> result;

	for (auto& alert : GetAll()) {
		if (alert->type == AlertType::Balance) {
			if (alert->condition == AlertCondition::Equal && alert->balance == alert->amount) {
				result.push_back(alert);
			}

			if (alert->condition == AlertCondition::Less && alert->balance < alert->amount) {
				result.push_back(alert);
			}

			if (alert->condition == AlertCondition::More && alert->balance > alert->amount) {
				result.push_back(alert);
			}
		}
	}

	return result;
}

void AlertsService::Reload() {
	_isLoading = false;
	_hash.clear();
	
	GetAll();
}

std::shared_ptr<AlertPresentationModel> AlertsService::Save(AlertPresentationModel& alert) {
	AlertModel& model = alert;

	int id = _alertsRepository.Save(model);

	delete& model;

	RemoveFromHash(id);

	_eventEmitter->Emit();

	return GetById(id);
}

void AlertsService::Delete(AlertPresentationModel& alert) {
	AlertModel& model = alert;

	_alertsRepository.Delete(model);

	delete& model;

	RemoveFromHash(alert.id);

	_eventEmitter->Emit();
}