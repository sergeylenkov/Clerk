#pragma once

#include <wx/dataview.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class BudgetsListDataModel : public wxDataViewListStore
{
public:
	enum
	{
		ColumnName,
		ColumnLimit,
		ColumnCurrent,
		ColumnRemain,
		ColumnProgress
	};

	BudgetsListDataModel();
	~BudgetsListDataModel();

	void SetItems(std::vector<std::shared_ptr<Budget>> budgets);
	std::shared_ptr<Budget> GetBudget(wxDataViewItem &item);

	virtual unsigned int GetColumnCount() const wxOVERRIDE;
	virtual wxString GetColumnType(unsigned int column) const wxOVERRIDE;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const wxOVERRIDE;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const wxOVERRIDE;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column) wxOVERRIDE;

private:
	std::vector<std::shared_ptr<Budget>> _budgets;
};

