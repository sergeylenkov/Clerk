#include "AlertsService.h"

AlertsService::AlertsService(AlertsRepository& alertsRepository, AccountsService& accountsService) :
	_alertsRepository(alertsRepository),
	_accountsService(accountsService) {
	_isLoading = false;
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

shared_vector<AlertPresentationModel> AlertsService::GetActive() {
	shared_vector<AlertPresentationModel> result;

	for (auto& alert : GetAll()) {
		if (alert->type == AlertType::Balance) {
			float total = 0;

			for (int id : alert->accountIds) {
				auto account = _accountsService.GetById(id);

				if (account) {
					total = account->balance;
				}
			}

			if (alert->condition == AlertCondition::Equal && total == alert->amount) {
				result.push_back(alert);
			}

			if (alert->condition == AlertCondition::Less && total < alert->amount) {
				result.push_back(alert);
			}

			if (alert->condition == AlertCondition::More && total > alert->amount) {
				result.push_back(alert);
			}
		}
	}

	return result;
}