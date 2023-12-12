#include "AlertPresentationModel.h"

using namespace Clerk::UI;

AlertPresentationModel::AlertPresentationModel() {
	id = -1;
	name = wxString("");
	message = wxString("");
	date = wxDateTime();
	type = AlertType::Balance;
	period = AlertPeriod::Month;
	condition = AlertCondition::Equal;
	importance = AlertImportance::Medium;
	amount = 0;
	balance = 0;
	isDismissed = false;
}

AlertPresentationModel::AlertPresentationModel(AlertModel& alert) {
	id = alert.id;
	name = wxString(alert.name);
	message = wxString(alert.message);
	date = wxDateTime().ParseFormat(alert.created);
	type = alert.type;
	period = alert.period;
	condition = alert.condition;
	importance = alert.importance;
	amount = alert.amount;
	accountIds = String::Split(alert.accountIds, ',');
	balance = 0;
	isDismissed = false;

	switch (alert.type)
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

	switch (alert.period)
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

	switch (alert.condition)
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

	switch (alert.importance)
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

	return *model;
}