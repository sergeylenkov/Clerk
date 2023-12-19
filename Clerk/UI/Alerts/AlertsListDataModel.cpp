#include "AlertsListDataModel.h"

AlertsListDataModel::AlertsListDataModel()
{
}

void AlertsListDataModel::SetItems(shared_vector<AlertPresentationModel> alerts) {
	_alerts = alerts;
	Reset(_alerts.size());
}

unsigned int AlertsListDataModel::GetColumnCount() const
{
	return 7;
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

	switch (static_cast<AlertsListColumns>(column))
	{
		case AlertsListColumns::Name:
			variant = alert->name;
			break;
		case AlertsListColumns::Message:
			variant = alert->message;
			break;
		case AlertsListColumns::Type:
			variant = alert->typeName;
			break;
		case AlertsListColumns::Period:
			variant = alert->periodName;
			break;
		case AlertsListColumns::Condition:
			variant = alert->conditionName;
			break;
		case AlertsListColumns::Importance:
			variant = alert->importanceName;
			break;
		case AlertsListColumns::Amount:
			variant = Format::Amount(alert->amount);
			break;
	}	
}

bool AlertsListDataModel::SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column) {
	return false;
}

int AlertsListDataModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const {
	wxUIntPtr index1 = wxPtrToUInt(item1.GetID()) - 1;
	wxUIntPtr index2 = wxPtrToUInt(item2.GetID()) - 1;

	auto v1 = _alerts[index1];
	auto v2 = _alerts[index2];

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Name) {
		return ascending ? v1->name.Cmp(v2->name) : v2->name.Cmp(v1->name);
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Message) {
		return ascending ? v1->message.Cmp(v2->message) : v2->message.Cmp(v1->message);
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Type) {
		if (v1->type == v2->type) {
			return 0;
		}

		return ascending ? v1->type > v2->type : v2->type > v1->type;
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Period) {
		if (v1->period == v2->period) {
			return 0;
		}

		return ascending ? v1->period > v2->period : v2->period > v1->period;
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Condition) {
		if (v1->condition == v2->condition) {
			return 0;
		}

		return ascending ? v1->condition > v2->condition : v2->condition > v1->condition;
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Importance) {
		if (v1->importance == v2->importance) {
			return 0;
		}

		return ascending ? v1->importance > v2->importance : v2->importance > v1->importance;
	}

	if (static_cast<AlertsListColumns>(column) == AlertsListColumns::Amount) {
		if (v1->amount == v2->amount) {
			return 0;
		}

		return ascending ? v1->amount > v2->amount : v2->amount > v1->amount;
	}

	return 0;
}

unsigned int AlertsListDataModel::GetCount() const {
	return _alerts.size();
}

bool AlertsListDataModel::HasDefaultCompare() const {
	return false;
}