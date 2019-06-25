#include "GoalsListDataModel.h"

GoalsListDataModel::GoalsListDataModel()
{
}

GoalsListDataModel::~GoalsListDataModel()
{
}

void GoalsListDataModel::SetItems(std::vector<std::shared_ptr<Goal>> goals) {
	_goals = goals;
	Reset(_goals.size());
}

unsigned int GoalsListDataModel::GetColumnCount() const
{
	return 8;
}

wxString GoalsListDataModel::GetColumnType(unsigned int column) const
{
	return wxT("string");
}

void GoalsListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const
{
	auto goal = _goals[row];

	float remainAmount = goal->amount - goal->balance;
	float remainPercent = goal->balance / (goal->amount / 100.0);

	switch (static_cast<Columns>(column))
	{
		case Columns::Name:
			variant = *goal->name;
			break;
		case Columns::DueDate:			
			variant = FormatDate(goal->date.get());
			break;
		case Columns::DaysRemain:
			variant = FormatDaysRemain(goal->date.get());
			break;
		case Columns::Goal:
			variant = Utils::FormatAmount(goal->amount);
			break;
		case Columns::Current:
			variant = Utils::FormatAmount(goal->balance);
			break;
		case Columns::Remain:
			variant = Utils::FormatAmount(remainAmount);
			break;
		case Columns::Progress:
			variant = wxString::Format("%f", remainPercent);
			break;
	}
}

bool GoalsListDataModel::GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const
{
	auto goal = _goals[row];
	float percent = goal->balance / (goal->amount / 100.0);

	switch (static_cast<Columns>(column))
	{
		case Columns::Remain:
			attr.SetColour(Utils::ColorForGoal(percent));
			return true;			
			break;
		case Columns::DaysRemain:
			if (goal->date->DiffAsDateSpan(wxDateTime::Now()).GetTotalDays() < 0) {
				attr.SetColour(wxColor(242, 73, 101));
				return true;
			}
			break;
	}

	return false;
}

bool GoalsListDataModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column)
{
	return false;
}

wxString GoalsListDataModel::FormatDate(wxDateTime *date) const
{
	wxString dateFormat = date->Format("%B %e");

	if (wxDateTime::Now().GetYear() != date->GetYear()) {
		dateFormat = date->Format("%B %e %Y");
	}

	return dateFormat;
}

wxString GoalsListDataModel::FormatDaysRemain(wxDateTime *date) const {
	wxDateSpan diff = date->DiffAsDateSpan(wxDateTime::Now());
	
	int days = diff.GetTotalDays();
	int months = diff.GetTotalMonths();

	if (months > 0) {
		return wxString::Format("%d m", months);
	}

	return wxString::Format("%d days", days);
}