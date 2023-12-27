#pragma once

#include <wx/wx.h>
#include "../PresentationModels/Types.h"
#include "../../Utils/Utils.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardGoalsPanel : public wxPanel
{
public:
	DashboardGoalsPanel(wxWindow *parent);

	void SetViewModel(DashboardViewModel* viewModel);
	

private:
	DashboardViewModel* _viewModel{};
	std::vector<DashboardProgressValue> _values;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
