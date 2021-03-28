#pragma once

#include <wx/dataview.h>
#include "../../Data/ViewModels/GoalViewModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;

class GoalsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		DueDate,
		DaysRemain,
		Goal,
		Current,
		Remain,
		Progress,
		Last
	};

	GoalsListDataModel();
	~GoalsListDataModel();

	void SetItems(std::vector<std::shared_ptr<GoalViewModel>> goals);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<GoalViewModel>> _goals;
	wxString FormatDate(wxDateTime& date) const;
	wxString FormatDaysRemain(wxDateTime& date) const;
};

