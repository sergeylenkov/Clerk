#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../PresentationModels/Types.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardReceiptsPanel : public wxPanel
{
public:
	DashboardReceiptsPanel(wxWindow* parent);

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};
	float _maxValue = 0;
	shared_vector<AccountPresentationModel> _accounts;
	float _total = 0.0;

	void Update();
	void Draw(wxPaintDC& dc);
	void OnPaint(wxPaintEvent& event);
};
