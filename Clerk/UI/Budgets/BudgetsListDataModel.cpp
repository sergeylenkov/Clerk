#include "BudgetsListDataModel.h"

BudgetsListDataModel::BudgetsListDataModel()
{
}

void BudgetsListDataModel::SetItems(shared_vector<BudgetPresentationModel> budgets) {
	_budgets = budgets;
	Reset(_budgets.size());
}

unsigned int BudgetsListDataModel::GetColumnCount() const
{
	return 6;
}

wxString BudgetsListDataModel::GetColumnType(unsigned int column) const
{
	return wxT("string");
}

void BudgetsListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const
{
	auto budget = _budgets[row];

	switch (static_cast<BudgetsListColumns>(column))
	{
		case BudgetsListColumns::Name:
			variant = budget->name;
			break;
		case BudgetsListColumns::Period:
			variant = budget->periodName;
			break;
		case BudgetsListColumns::Limit:
			variant = Format::Amount(budget->amount);
			break;
		case BudgetsListColumns::Current:
			variant = Format::Amount(budget->balance);
			break;
		case BudgetsListColumns::Remain:
			variant = Format::Amount(budget->remainAmount);
			break;
		case BudgetsListColumns::Progress:
			variant = wxString::Format("%f", budget->remainPercent);
			break;
	}	
}

bool BudgetsListDataModel::GetAttrByRow(unsigned int row, unsigned int column,	wxDataViewItemAttr &attr) const
{
	auto budget = _budgets[row];

	switch (static_cast<BudgetsListColumns>(column))
	{
		case BudgetsListColumns::Remain:
			attr.SetColour(Colors::ColorForBudget(budget->remainPercent));
			return true;			
			break;
	}

	return false;
}

bool BudgetsListDataModel::SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column) {
	return false;
}

int BudgetsListDataModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const {
	wxUIntPtr index1 = wxPtrToUInt(item1.GetID()) - 1;
	wxUIntPtr index2 = wxPtrToUInt(item2.GetID()) - 1;

	auto v1 = _budgets[index1];
	auto v2 = _budgets[index2];

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Name) {
		return ascending ? v1->name.Cmp(v2->name) : v2->name.Cmp(v1->name);
	}

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Period) {
		if (v1->period == v2->period) {
			return 0;
		}

		return ascending ? v1->period > v2->period : v2->period > v1->period;
	}

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Current) {
		if (v1->amount == v2->amount) {
			return 0;
		}

		return ascending ? v1->amount > v2->amount : v2->amount > v1->amount;
	}

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Limit) {
		if (v1->balance == v2->balance) {
			return 0;
		}

		return ascending ? v1->balance > v2->balance : v2->balance > v1->balance;
	}

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Remain) {
		if (v1->remainAmount == v2->remainAmount) {
			return 0;
		}

		return ascending ? v1->remainAmount > v2->remainAmount : v2->remainAmount > v1->remainAmount;
	}

	if (static_cast<BudgetsListColumns>(column) == BudgetsListColumns::Progress) {
		if (v1->remainPercent == v2->remainPercent) {
			return 0;
		}

		return ascending ? v1->remainPercent > v2->remainPercent : v2->remainPercent > v1->remainPercent;
	}

	return 0;
}

unsigned int BudgetsListDataModel::GetCount() const {
	return _budgets.size();
}

bool BudgetsListDataModel::HasDefaultCompare() const {
	return false;
}