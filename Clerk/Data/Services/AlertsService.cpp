#include "AlertsService.h"

AlertsService::AlertsService(AlertsRepository& alertsRepository) : _alertsRepository(alertsRepository) {
	_isLoading = false;
}

AlertsService::~AlertsService() {
	_hash.clear();
	_list.clear();
}

std::shared_ptr<AlertPresentationModel> AlertsService::GetById(int id) {
	auto alert = GetFromHash(id);

	if (alert) {
		return alert;
	}

	auto model = _alertsRepository.GetById(id);

	if (model) {
		alert = std::make_shared<AlertPresentationModel>(*model);
		alert->balance = _alertsRepository.GetBalance(*alert);

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

			alert->balance = _alertsRepository.GetBalance(*alert);

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