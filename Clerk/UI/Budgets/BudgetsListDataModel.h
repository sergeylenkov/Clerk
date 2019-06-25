#pragma once

#include <wx/dataview.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class BudgetsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		Period,
		Limit,
		Current,
		Remain,
		Progress,
		Last
	};

	BudgetsListDataModel();
	~BudgetsListDataModel();

	void SetItems(std::vector<std::shared_ptr<Budget>> budgets);
	
	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<Budget>> _budgets;
};

