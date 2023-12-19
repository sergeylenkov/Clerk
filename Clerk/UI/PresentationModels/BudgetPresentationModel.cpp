#include "BudgetPresentationModel.h"

using namespace Clerk::UI;

BudgetPresentationModel::BudgetPresentationModel(BudgetModel& budget) {
	id = budget.id;
	name = budget.name;
	date = wxDateTime();
	periodDate = wxDateTime::Today();
	amount = budget.amount;
	date.ParseISODate(budget.date);
	remainAmount = 0;
	remainPercent = 0;

	accountsIds = String::Split(budget.accountsIds, ',');

	switch (budget.period)
	{
		case BudgetPeriod::Week:
			periodName = wxString("Week");
			periodDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);

			break;
		case BudgetPeriod::Month:
			periodName = wxString("Month");
			periodDate.SetDay(1);

			break;
		case BudgetPeriod::Year:
			periodName = wxString("Year");

			periodDate.SetMonth(wxDateTime::Month::Jan);
			periodDate.SetDay(1);

			break;
		case BudgetPeriod::Custom:
			periodName = wxString("Custom");
			break;
		default:
			periodName = wxString("");
			break;
	}
}

BudgetPresentationModel::operator BudgetModel& () {
	return GetModel();
}

BudgetModel& BudgetPresentationModel::GetModel() {
	BudgetModel* model = new BudgetModel();

	model->id = id;
	model->name = name;
	model->amount = amount;
	model->period = period;
	model->date = date.FormatISODate();

	std::vector<std::string> res;

	for (int id : accountsIds) {
		res.push_back(std::to_string(id));
	}

	std::string ids = String::Join(res, ",");

	model->accountsIds = std::wstring(ids.begin(), ids.end());

	return *model;
}