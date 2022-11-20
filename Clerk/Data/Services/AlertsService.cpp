#include "AlertsService.h"

using namespace Clerk::Data;

AlertsService::AlertsService(AlertsRepository& alertsRepository) : _alertsRepository(alertsRepository) {

}

std::shared_ptr<AlertPresentationModel> AlertsService::GetById(int id) {
	auto alert = _alertsRepository.GetById(id);

	if (alert) {
		return std::make_shared<AlertPresentationModel>(*alert);
	}

	return nullptr;
}

std::vector<std::shared_ptr<AlertPresentationModel>> AlertsService::GetAll() {
	auto alerts = _alertsRepository.GetAll();

	std::vector<std::shared_ptr<AlertPresentationModel>> result;

	std::transform(alerts.begin(), alerts.end(), std::back_inserter(result), [&](const std::shared_ptr<AlertModel>& alert) {
		auto model = std::make_shared<AlertPresentationModel>(*alert);
		model->balance = _alertsRepository.GetBalance(*alert);

		return model;
	});

	return result;
}