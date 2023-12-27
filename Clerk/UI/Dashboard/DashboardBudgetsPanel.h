#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Data/PresentationModels/BudgetPresentationModel.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

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



