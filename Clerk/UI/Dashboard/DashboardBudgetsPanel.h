#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <map>
#include "../../Data//DataHelper.h"

class DashboardBudgetsPanel : public wxPanel
{
public:
	DashboardBudgetsPanel(wxWindow *parent, wxWindowID id);
	~DashboardBudgetsPanel();

	void SetBudgets(std::vector<std::shared_ptr<Budget>> budgets);
	void Update();

private:
	std::vector<std::shared_ptr<Budget>> budgets;
	std::vector<DahboardBudgetValue> values;
	int daysCount;
	int currentDay;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};



