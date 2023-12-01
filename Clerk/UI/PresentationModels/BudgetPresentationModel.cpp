#include "BudgetPresentationModel.h"

using namespace Clerk::UI;

BudgetPresentationModel::BudgetPresentationModel(BudgetModel& budget) {
	this->id = budget.id;
	this->name = budget.name;
	this->date = wxDateTime();
	this->periodDate = wxDateTime::Today();
	this->amount = budget.amount;

	this->date.ParseISODate(budget.date);

	std::wstringstream ss(budget.accountsIds);

	int i;

	while (ss >> i)
	{
		this->accountsIds.push_back(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	switch (budget.period)
	{
		case BudgetPeriod::Week:
			this->periodName = wxString("Week");
			this->periodDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);

			break;
		case BudgetPeriod::Month:
			this->periodName = wxString("Month");
			this->periodDate.SetDay(1);

			break;
		case BudgetPeriod::Year:
			this->periodName = wxString("Year");

			this->periodDate.SetMonth(wxDateTime::Month::Jan);
			this->periodDate.SetDay(1);

			break;
		case BudgetPeriod::Custom:
			this->periodName = wxString("Custom");
			break;
		default:
			this->periodName = wxString("");
			break;
	}
}

BudgetPresentationModel::operator BudgetModel& () {
	return GetModel();
}

wxString BudgetPresentationModel::GetAccountsIdsString() {
	wxString result = "";

	if (accountsIds.size() > 0) {
		for (unsigned int i = 0; i < accountsIds.size() - 1; i++) {
			result.Append(std::to_string(accountsIds[i]));
			result.Append(",");
		}

		result.Append(std::to_string(accountsIds[accountsIds.size() - 1]));
	}

	return result;
}

BudgetModel& BudgetPresentationModel::GetModel() {
	BudgetModel* model = new BudgetModel();

	model->id = id;
	model->name = name;
	model->amount = amount;
	model->period = period;
	model->date = date.FormatISODate();
	model->accountsIds = GetAccountsIdsString();

	return *model;
}