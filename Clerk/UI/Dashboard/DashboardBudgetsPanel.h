#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/BudgetViewModel.h"
#include "../../Data/ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardBudgetsPanel : public wxPanel
{
public:
	DashboardBudgetsPanel(wxWindow *parent);	
	
	void SetViewModel(DashboardViewModel* viewModel);	

private:
	DashboardViewModel* _viewModel{};
	int _daysCount;
	int _currentDay;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};



