#include "SchedulersListDataModel.h"

SchedulersListDataModel::SchedulersListDataModel()
{
}

void SchedulersListDataModel::SetItems(shared_vector<SchedulerPresentationModel> schedulers) {
	_schedulers = schedulers;
	Reset(_schedulers.size());
}

unsigned int SchedulersListDataModel::GetColumnCount() const
{
	return 7;
}

wxString SchedulersListDataModel::GetColumnType(unsigned int column) const
{
	return wxT("string");
}

void SchedulersListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const
{
	auto scheduler = _schedulers[row];

	switch (static_cast<SchedulersListColumns>(column))
	{
		case SchedulersListColumns::Name:
			variant = scheduler->name;
			break;
		case SchedulersListColumns::Type:
			variant = scheduler->typeName;
			break;
		case SchedulersListColumns::NextDate:
			variant = Format::Date(scheduler->nextDate);
			break;
		case SchedulersListColumns::DaysLeft:
			variant = Format::DaysRemain(scheduler->nextDate);
			break;
		case SchedulersListColumns::Amount:
			variant = Format::Amount(scheduler->fromAmount);
			break;
		case SchedulersListColumns::Status:
			if (scheduler->isActive) {
				variant = _("Active");
			}
			else {
				variant = _("Paused");
			}
			break;
	}
}

bool SchedulersListDataModel::GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const
{
	auto scheduler = _schedulers[row];

	switch (static_cast<SchedulersListColumns>(column))
	{
		case SchedulersListColumns::Status:
			if (!scheduler->isActive) {
				attr.SetColour(wxColor(110, 110, 110));
				return true;
			}
			break;
	}

	return false;
}

bool SchedulersListDataModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column)
{
	return false;
}

int SchedulersListDataModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const {
	wxUIntPtr index1 = wxPtrToUInt(item1.GetID()) - 1;
	wxUIntPtr index2 = wxPtrToUInt(item2.GetID()) - 1;

	auto v1 = _schedulers[index1];
	auto v2 = _schedulers[index2];

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::Name) {
		return ascending ? v1->name.Cmp(v2->name) : v2->name.Cmp(v1->name);
	}

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::Type) {
		if (v1->type == v2->type) {
			return 0;
		}

		return ascending ? v1->type > v2->type : v2->type > v1->type;
	}

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::Amount) {
		if (v1->fromAmount == v2->fromAmount) {
			return 0;
		}

		return ascending ? v1->fromAmount > v2->fromAmount : v2->fromAmount > v1->fromAmount;
	}

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::NextDate) {
		if (v1->nextDate.IsEqualTo(v2->nextDate)) {
			return 0;
		}

		return (ascending == v1->nextDate.IsLaterThan(v2->nextDate)) ? 1 : -1;
	}

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::DaysLeft) {
		if (v1->nextDate.IsEqualTo(v2->nextDate)) {
			return 0;
		}

		return (ascending == v1->nextDate.IsLaterThan(v2->nextDate)) ? 1 : -1;
	}

	if (static_cast<SchedulersListColumns>(column) == SchedulersListColumns::Status) {
		if (v1->isActive == v2->isActive) {
			return 0;
		}

		return ascending ? v1->isActive > v2->isActive : v2->isActive > v1->isActive;
	}

	return 0;
}

unsigned int SchedulersListDataModel::GetCount() const {
	return _schedulers.size();
}

bool SchedulersListDataModel::HasDefaultCompare() const {
	return false;
}