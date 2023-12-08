#include "AlertPresentationModel.h"

using namespace Clerk::UI;

AlertPresentationModel::AlertPresentationModel(AlertModel& alert) {
	id = alert.id;
	name = alert.name;
	date = wxDateTime().ParseFormat(alert.created);
	type = alert.type;
	period = alert.period;
	condition = alert.condition;
	amount = alert.amount;
	balance = 0;

	std::wstringstream ss(alert.accountIds);

	int i;

	while (ss >> i)
	{
		accountIds.push_back(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

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