#include "BudgetPresentationModel.h"

using namespace Clerk::Data;

BudgetPresentationModel::BudgetPresentationModel() {
	id = -1;
	name = wxString("");
	period = BudgetPeriod::Month;
	date = wxDateTime::Today();
	periodDate = wxDateTime::Today();
	amount = 0;
	balance = 0;
	remainAmount = 0;
	remainPercent = 0;
	created = wxDateTime::Now();
}

BudgetPresentationModel::BudgetPresentationModel(BudgetModel& model) {
	id = model.id;
	name = model.name;
	period = model.period;

	date = wxDateTime::Today();
	date.ParseISODate(model.date);

	periodDate = wxDateTime::Today();
	amount = model.amount;
	balance = 0;
	remainAmount = 0;
	remainPercent = 0;	
	accountsIds = String::Split(model.accountsIds, ',');

	created = wxDateTime::Now();
	created.ParseISODate(model.created);

	switch (model.period)
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
	model->created = created.FormatISOCombined(' ');

	std::string ids = String::Join(accountsIds, ",");

	model->accountsIds = std::wstring(ids.begin(), ids.end());

	return *model;
}