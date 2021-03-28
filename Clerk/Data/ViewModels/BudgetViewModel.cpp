#include "BudgetViewModel.h"

using namespace Clerk::Data;

BudgetViewModel::BudgetViewModel(BudgetModel& budget) {
	this->id = budget.id;
	this->name = wxString::FromUTF8(budget.name.c_str());
	this->date = wxDateTime();
	this->periodDate = wxDateTime::Today();
	this->amount = budget.amount;

	this->date.ParseISODate(budget.date);

	std::stringstream ss(budget.accountIds);

	int i;

	while (ss >> i)
	{
		this->accountIds.push_back(i);

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