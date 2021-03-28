#include "SchedulersListDataModel.h"

SchedulersListDataModel::SchedulersListDataModel()
{
}

SchedulersListDataModel::~SchedulersListDataModel()
{
}

void SchedulersListDataModel::SetItems(std::vector<std::shared_ptr<SchedulerViewModel>> schedulers) {
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

	switch (static_cast<Columns>(column))
	{
		case Columns::Name:
			variant = scheduler->name;
			break;
		case Columns::Type:
			variant = scheduler->typeName;
			break;
		case Columns::NextDate:
			variant = FormatDate(scheduler->nextDate);
			break;
		case Columns::DaysLeft:
			variant = FormatDaysLeft(scheduler->nextDate);
			break;
		case Columns::Amount:
			variant = Format::Amount(scheduler->fromAmount);
			break;
		case Columns::Status:
			if (scheduler->isActive) {
				variant = "Active";
			}
			else {
				variant = "Paused";
			}
			break;
	}
}

bool SchedulersListDataModel::GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const
{
	auto scheduler = _schedulers[row];

	switch (static_cast<Columns>(column))
	{
		case Columns::Status:
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


wxString SchedulersListDataModel::FormatDate(const wxDateTime& date) const
{
	wxString dateFormat = date.Format("%B %e");

	if (wxDateTime::Now().GetYear() != date.GetYear()) {
		dateFormat = date.Format("%B %e, %Y");
	}

	return dateFormat;
}

wxString SchedulersListDataModel::FormatDaysLeft(const wxDateTime& date) const {
	wxDateSpan diff = date.DiffAsDateSpan(wxDateTime::Now());

	int days = diff.GetTotalDays();
	int months = diff.GetTotalMonths();

	if (months > 0) {
		return wxString::Format("%d months", months);
	}

	return wxString::Format("%d days", days);
}