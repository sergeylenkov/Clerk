#pragma once

#include <wx/dataview.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class GoalsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		DueDate,
		Goal,
		Current,
		Remain,
		Progress,
		Last
	};

	GoalsListDataModel();
	~GoalsListDataModel();

	void SetItems(std::vector<std::shared_ptr<Goal>> goals);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<Goal>> _goals;
	wxString FormatDate(wxDateTime *date) const;
};

