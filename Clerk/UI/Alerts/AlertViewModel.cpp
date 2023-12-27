#include "AlertViewModel.h"

using namespace Clerk::UI;

AlertViewModel::AlertViewModel(AlertsService& alertsService, AccountsService& accountsService):
	_alertsService(alertsService),
	_accountsService(accountsService)
{
	_id = -1;
	_type = AlertType::Balance;
	_period = AlertPeriod::Month;
	_condition = AlertCondition::Equal;
	_importance = AlertImportance::Medium;
	_amount = 0;
}

void AlertViewModel::SetAlertId(int id) {
	auto alert = _alertsService.GetById(id);

	if (alert) {
		_id = alert->id;
		_name = alert->name;
		_message = alert->message;
		_type = alert->type;
		_period = alert->period;
		_condition = alert->condition;
		_importance = alert->importance;
		_amount = alert->amount;
		_accountsIds = alert->accountsIds;
	}
}

bool AlertViewModel::IsNew() {
	return _id == -1;
}

void AlertViewModel::SetName(wxString name) {
	_name = name;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Name);
	}
}

wxString AlertViewModel::GetName() {
	return _name;
}

void AlertViewModel::SetMessage(wxString message) {
	_message = message;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Name);
	}
}

wxString AlertViewModel::GetMessage() {
	return _message;
}

void AlertViewModel::SetType(AlertType type) {
	_type = type;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Type);
	}
}

AlertType AlertViewModel::GetType() {
	return _type;
}

void AlertViewModel::SetPeriod(AlertPeriod period) {
	_period = period;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Period);
	}
}

AlertPeriod AlertViewModel::GetPeriod() {
	return _period;
}

void AlertViewModel::SetCondition(AlertCondition condition) {
	_condition = condition;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Condition);
	}
}

AlertCondition AlertViewModel::GetCondition() {
	return _condition;
}

void AlertViewModel::SetImportance(AlertImportance importance) {
	_importance = importance;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Importance);
	}
}

AlertImportance AlertViewModel::GetImportance() {
	return _importance;	
}

void AlertViewModel::SetAmount(float amount) {
	_amount = amount;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Amount);
	}
}

float AlertViewModel::GetAmount() {
	return _amount;
}

void AlertViewModel::SetAccountsIds(std::vector<int> ids) {
	_accountsIds = ids;

	if (OnUpdate) {
		OnUpdate(AlertViewModelField::Accounts);
	}
}

std::vector<int> AlertViewModel::GetAccountsIds() {
	return _accountsIds;
}

shared_vector<AccountPresentationModel> AlertViewModel::GetAccounts() {
	if (_type == AlertType::Receipt) {
		return _accountsService.GetReceipts();
	}
	else if (_type == AlertType::Expense) {
		return _accountsService.GetExpenses();
	}

	return _accountsService.GetDepositsAndVirtuals();
}

void AlertViewModel::Save() {
	auto alert = _alertsService.GetById(_id);

	if (!alert) {
		alert = std::make_shared<AlertPresentationModel>();
	}

	alert->name = _name;	
	alert->message = _message;
	alert->type = _type;
	alert->period = _period;
	alert->condition = _condition;
	alert->importance = _importance;
	alert->amount = _amount;
	alert->accountsIds = _accountsIds;

	_alertsService.Save(*alert);
}