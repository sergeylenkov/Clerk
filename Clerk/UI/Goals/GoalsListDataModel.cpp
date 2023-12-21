#include "GoalsListDataModel.h"

GoalsListDataModel::GoalsListDataModel()
{
}

void GoalsListDataModel::SetItems(shared_vector<GoalPresentationModel> goals) {
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

	switch (static_cast<GoalsListColumns>(column))
	{
		case GoalsListColumns::Name:
			variant = goal->name;
			break;
		case GoalsListColumns::DueDate:
			variant = Format::Date(goal->date);
			break;
		case GoalsListColumns::DaysRemain:
			variant = Format::DaysRemain(goal->date);
			break;
		case GoalsListColumns::Goal:
			variant = Format::Amount(goal->amount);
			break;
		case GoalsListColumns::Current:
			variant = Format::Amount(goal->balance);
			break;
		case GoalsListColumns::Remain:
			variant = Format::Amount(goal->remainAmount);
			break;
		case GoalsListColumns::Progress:
			variant = wxString::Format("%f", goal->remainPercent);
			break;
	}
}

bool GoalsListDataModel::GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const
{
	auto goal = _goals[row];

	switch (static_cast<GoalsListColumns>(column))
	{
		case GoalsListColumns::Remain:
			attr.SetColour(Colors::ColorForGoal(goal->remainPercent));
			return true;			
			break;
		case GoalsListColumns::DaysRemain:
			if (goal->date.DiffAsDateSpan(wxDateTime::Now()).GetTotalDays() < 0) {
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

int GoalsListDataModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const {
	wxUIntPtr index1 = wxPtrToUInt(item1.GetID()) - 1;
	wxUIntPtr index2 = wxPtrToUInt(item2.GetID()) - 1;

	auto v1 = _goals[index1];
	auto v2 = _goals[index2];

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::Name) {
		return ascending ? v1->name.Cmp(v2->name) : v2->name.Cmp(v1->name);
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::Goal) {
		if (v1->amount == v2->amount) {
			return 0;
		}

		return ascending ? v1->amount > v2->amount : v2->amount > v1->amount;
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::Current) {
		if (v1->balance == v2->balance) {
			return 0;
		}

		return ascending ? v1->balance > v2->balance : v2->balance > v1->balance;
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::DueDate) {
		if (v1->date.IsEqualTo(v2->date)) {
			return 0;
		}

		return (ascending == v1->date.IsLaterThan(v2->date)) ? 1 : -1;
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::DaysRemain) {
		if (v1->date.IsEqualTo(v2->date)) {
			return 0;
		}

		return (ascending == v1->date.IsLaterThan(v2->date)) ? 1 : -1;
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::Remain) {
		if (v1->remainAmount == v2->remainAmount) {
			return 0;
		}

		return ascending ? v1->remainAmount > v2->remainAmount : v2->remainAmount > v1->remainAmount;
	}

	if (static_cast<GoalsListColumns>(column) == GoalsListColumns::Progress) {
		if (v1->remainPercent == v2->remainPercent) {
			return 0;
		}

		return ascending ? v1->remainPercent > v2->remainPercent : v2->remainPercent > v1->remainPercent;
	}

	return 0;
}

unsigned int GoalsListDataModel::GetCount() const {
	return _goals.size();
}

bool GoalsListDataModel::HasDefaultCompare() const {
	return false;
}