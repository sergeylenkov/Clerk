#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/ViewModels/Types.h"
#include "../../Data/ViewModels/GoalViewModel.h"
#include "../../Data/ViewModels/DashboardViewModel.h"
#include "../../Utils/Utils.h"

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
	std::vector<std::shared_ptr<GoalViewModel>> _goals;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
