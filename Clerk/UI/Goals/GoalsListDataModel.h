#pragma once

#include <wx/dataview.h>
#include "../PresentationModels/GoalPresentationModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

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

	void SetItems(std::vector<std::shared_ptr<GoalPresentationModel>> goals);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<GoalPresentationModel>> _goals;
	wxString FormatDate(wxDateTime& date) const;
	wxString FormatDaysRemain(wxDateTime& date) const;
};

