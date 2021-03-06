#pragma once

#include <wx/dataview.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/BudgetViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

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

	void SetItems(std::vector<std::shared_ptr<BudgetViewModel>> budgets);
	
	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<BudgetViewModel>> _budgets;
};

