#include "AlertPresentationModel.h"

using namespace Clerk::UI;

AlertPresentationModel::AlertPresentationModel(AlertModel& alert) {
	this->id = alert.id;
	this->name = alert.name;
	this->date = wxDateTime().ParseFormat(alert.created);
	this->type = alert.type;
	this->period = alert.period;
	this->condition = alert.condition;
	
	std::wstringstream ss(alert.accountIds);

	int i;

	while (ss >> i)
	{
		this->accountIds.push_back(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	switch (alert.type)
	{
		case AlertType::Expense:
			this->typeName = wxString("Expense");
			break;
		case AlertType::Receipt:
			this->typeName = wxString("Receipt");
			break;
		case AlertType::Balance:
			this->typeName = wxString("Balance");
			break;
		default:
			break;
	}

	switch (alert.period)
	{
		case AlertPeriod::Week:
			this->periodName = wxString("Week");
			break;
		case AlertPeriod::Month:
			this->periodName = wxString("Month");
			break;
		case AlertPeriod::Year:
			this->periodName = wxString("Year");
			break;
		default:
			break;
	}

	switch (alert.condition)
	{
		case AlertCondition::Less:
			this->conditionName = wxString("Less");
			break;
		case AlertCondition::More:
			this->conditionName = wxString("More");
			break;
		case AlertCondition::Equal:
			this->conditionName = wxString("Equal");
			break;
		default:
			break;
	}
}