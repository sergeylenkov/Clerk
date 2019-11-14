#include "AlertsListDataModel.h"

AlertsListDataModel::AlertsListDataModel()
{
}

AlertsListDataModel::~AlertsListDataModel()
{
}

void AlertsListDataModel::SetItems(std::vector<std::shared_ptr<Alert>> alerts) {
	_alerts = alerts;
	Reset(_alerts.size());
}

unsigned int AlertsListDataModel::GetColumnCount() const
{
	return 6;
}

wxString AlertsListDataModel::GetColumnType(unsigned int column) const
{
	return wxT("string");
}

void AlertsListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const
{
	auto alert = _alerts[row];

	float remainAmount = 0;
	float remainPercent = 0;

	switch (static_cast<Columns>(column))
	{
		case Columns::Name:			
			variant = *alert->name;
			break;
		case Columns::Type:
			variant = *alert->typeName;
			break;
		case Columns::Period:
			variant = *alert->periodName;
			break;
		case Columns::Condition:
			variant = *alert->conditionName;
			break;
		case Columns::Amount:
			variant = Utils::FormatAmount(alert->amount);
			break;
	}	
}

bool AlertsListDataModel::GetAttrByRow(unsigned int row, unsigned int column,	wxDataViewItemAttr &attr) const
{
	return false;
}

bool AlertsListDataModel::SetValueByRow(const wxVariant &variant,	unsigned int row, unsigned int column)
{
	return false;
}
