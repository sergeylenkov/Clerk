#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/CurrencyValueViewModel.h"
#include "../../Data/ViewModels/DashboardViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent);
	~DashboardBalancePanel();

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
