#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include "../../Utils/Utils.h"
#include "../../Data/PresentationModels/Types.h"
#include "DashboardViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent);

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};
	float _total = 0.0;
	float _ownFunds = 0.0;
	float _creditFunds = 0.0;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
