#include "AlertPresentationModel.h"

using namespace Clerk::Data;

AlertPresentationModel::AlertPresentationModel() {
	id = -1;
	name = wxString("");
	message = wxString("");
	type = AlertType::Balance;
	period = AlertPeriod::Month;
	condition = AlertCondition::Equal;
	importance = AlertImportance::Medium;
	amount = 0;
	balance = 0;
	created = wxDateTime::Now();
}

AlertPresentationModel::AlertPresentationModel(AlertModel& model) {
	id = model.id;
	name = wxString(model.name);
	message = wxString(model.message);
	type = model.type;
	period = model.period;
	condition = model.condition;
	importance = model.importance;
	amount = model.amount;
	accountsIds = String::Split(model.accountIds, ',');
	balance = 0;

	created = wxDateTime::Now();
	created.ParseISODate(model.created);

	switch (model.type)
	{
		case AlertType::Expense:
			typeName = _("Expense");
			break;
		case AlertType::Receipt:
			typeName = _("Receipt");
			break;
		case AlertType::Balance:
			typeName = _("Balance");
			break;
		default:
			break;
	}

	switch (model.period)
	{
		case AlertPeriod::Week:
			periodName = _("Week");
			break;
		case AlertPeriod::Month:
			periodName = _("Month");
			break;
		case AlertPeriod::Year:
			periodName = _("Year");
			break;
		default:
			break;
	}

	switch (model.condition)
	{
		case AlertCondition::Less:
			conditionName = _("Less");
			break;
		case AlertCondition::More:
			conditionName = _("More");
			break;
		case AlertCondition::Equal:
			conditionName = _("Equal");
			break;
		default:
			break;
	}

	switch (model.importance)
	{
		case AlertImportance::Low:
			importanceName = _("Low");
			break;
		case AlertImportance::Medium:
			importanceName = _("Medium");
			break;
		case AlertImportance::High:
			importanceName = _("High");
			break;
		default:
			break;
	}
}

AlertPresentationModel::operator AlertModel& () {
	return GetModel();
}

AlertModel& AlertPresentationModel::GetModel() {
	AlertModel* model = new AlertModel();

	model->id = id;
	model->name = name;
	model->message = message;
	model->type = type;
	model->period = period;
	model->condition = condition;
	model->importance = importance;	
	model->amount = amount;
	model->created = created.FormatISOCombined(' ');

	std::string ids = String::Join(accountsIds, ",");

	model->accountIds = std::wstring(ids.begin(), ids.end());

	return *model;
}