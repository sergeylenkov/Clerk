#include "BudgetsListDataModel.h"

BudgetsListDataModel::BudgetsListDataModel()
{
}

BudgetsListDataModel::~BudgetsListDataModel()
{
}

void BudgetsListDataModel::SetItems(std::vector<std::shared_ptr<Budget>> budgets) {
	_budgets = budgets;
	Reset(_budgets.size());
}

shared_ptr<Budget> BudgetsListDataModel::GetBudget(wxDataViewItem &item) {
	return nullptr;
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

	float remainAmount = budget->amount - budget->balance;
	float remainPercent = budget->balance / (budget->amount / 100.0);

	switch (column)
	{
		case ColumnName:			
			variant = *budget->name;
			break;
		case ColumnPeriod:
			variant = *budget->periodName;
			break;
		case ColumnLimit:
			variant = Utils::FormatAmount(budget->amount);
			break;
		case ColumnCurrent:
			variant = Utils::FormatAmount(budget->balance);
			break;
		case ColumnRemain:
			variant = Utils::FormatAmount(remainAmount);
			break;
		case ColumnProgress:
			variant = wxString::Format("%f", remainPercent);
			break;
	}	
}

bool BudgetsListDataModel::GetAttrByRow(unsigned int row, unsigned int column,	wxDataViewItemAttr &attr) const
{
	auto budget = _budgets[row];
	float percent = budget->balance / (budget->amount / 100.0);

	switch (column)
	{
		case ColumnRemain:
			if (percent > 90) {
				attr.SetColour(wxColor(242, 73, 101));
				return true;
			}
			break;
	}

	return false;
}

bool BudgetsListDataModel::SetValueByRow(const wxVariant &variant,	unsigned int row, unsigned int column)
{
	return false;
}
