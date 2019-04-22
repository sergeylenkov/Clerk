#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"

class DashboardGoalsPanel : public wxPanel
{
public:
	DashboardGoalsPanel(wxWindow *parent, wxWindowID id);
	~DashboardGoalsPanel();

	void SetGoals(std::vector<std::shared_ptr<Goal>> goals);
	void Update();

private:
	std::vector<std::shared_ptr<Goal>> goals;
	std::vector<DahboardBudgetValue> values;
	int daysCount;
	int currentDay;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};
