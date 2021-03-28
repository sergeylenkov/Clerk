#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/ViewModels/Types.h"
#include "../../Data/ViewModels/GoalViewModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardGoalsPanel : public wxPanel
{
public:
	DashboardGoalsPanel(wxWindow *parent);

	void SetGoals(std::vector<std::shared_ptr<GoalViewModel>> goals);
	void Update();

private:
	std::vector<std::shared_ptr<GoalViewModel>> _goals;
	std::vector<DashboardProgressValue> _values;
	
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
