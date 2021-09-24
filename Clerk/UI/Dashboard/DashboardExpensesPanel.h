#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/CurrencyValueViewModel.h"
#include "../../Data/ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardExpensesPanel : public wxPanel
{
public:
	DashboardExpensesPanel(wxWindow *parent);

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel;
	float maxValue;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
