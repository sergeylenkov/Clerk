#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../PresentationModels/BudgetPresentationModel.h"
#include "../ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardBudgetsPanel : public wxPanel
{
public:
	DashboardBudgetsPanel(wxWindow *parent);	
	
	void SetViewModel(DashboardViewModel* viewModel);	

private:
	DashboardViewModel* _viewModel{};
	std::vector<std::shared_ptr<BudgetPresentationModel>> _budgets;
	int _daysCount = 0;
	int _currentDay = 0;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};



