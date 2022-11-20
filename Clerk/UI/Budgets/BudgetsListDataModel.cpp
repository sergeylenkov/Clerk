#include "BudgetsListDataModel.h"

BudgetsListDataModel::BudgetsListDataModel()
{
}

BudgetsListDataModel::~BudgetsListDataModel()
{
}

void BudgetsListDataModel::SetItems(std::vector<std::shared_ptr<BudgetPresentationModel>> budgets) {
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

	float remainAmount = budget->amount - budget->balance;
	float remainPercent = budget->balance / (budget->amount / 100.0);

	switch (static_cast<Columns>(column))
	{
		case Columns::Name:			
			variant = budget->name;
			break;
		case Columns::Period:
			variant = budget->periodName;
			break;
		case Columns::Limit:
			variant = Format::Amount(budget->amount);
			break;
		case Columns::Current:
			variant = Format::Amount(budget->balance);
			break;
		case Columns::Remain:
			variant = Format::Amount(remainAmount);
			break;
		case Columns::Progress:
			variant = wxString::Format("%f", remainPercent);
			break;
	}	
}

bool BudgetsListDataModel::GetAttrByRow(unsigned int row, unsigned int column,	wxDataViewItemAttr &attr) const
{
	auto budget = _budgets[row];
	float percent = budget->balance / (budget->amount / 100.0);

	switch (static_cast<Columns>(column))
	{
		case Columns::Remain:			
			attr.SetColour(Colors::ColorForBudget(percent));
			return true;			
			break;
	}

	return false;
}

bool BudgetsListDataModel::SetValueByRow(const wxVariant &variant,	unsigned int row, unsigned int column)
{
	return false;
}
