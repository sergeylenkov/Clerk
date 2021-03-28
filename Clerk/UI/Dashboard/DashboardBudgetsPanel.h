#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/BudgetViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardBudgetsPanel : public wxPanel
{
public:
	DashboardBudgetsPanel(wxWindow *parent);
	
	void SetBudgets(std::vector<std::shared_ptr<BudgetViewModel>> budgets);
	void Update();

private:
	std::vector<std::shared_ptr<BudgetViewModel>> _budgets;
	int _daysCount;
	int _currentDay;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};



