#pragma once

#include <wx/dataview.h>
#include "../../Data/ViewModels/SchedulerViewModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class SchedulersListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		Type,
		Amount,
		NextDate,
		DaysLeft,
		Status,
		Last
	};

	SchedulersListDataModel();
	~SchedulersListDataModel();

	void SetItems(std::vector<std::shared_ptr<SchedulerViewModel>> schedulers);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<SchedulerViewModel>> _schedulers;
	wxString FormatDate(const wxDateTime& date) const;
	wxString FormatDaysLeft(const wxDateTime& date) const;
};

