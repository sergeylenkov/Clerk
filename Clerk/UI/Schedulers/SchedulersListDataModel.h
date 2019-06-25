#pragma once

#include <wx/dataview.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

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

	void SetItems(std::vector<std::shared_ptr<Scheduler>> schedulers);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<Scheduler>> _schedulers;
	wxString FormatDate(wxDateTime *date) const;
	wxString FormatDaysLeft(wxDateTime *date) const;
};

