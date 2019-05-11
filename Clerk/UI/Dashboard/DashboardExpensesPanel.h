#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data//DataHelper.h"

class DashboardExpensesPanel : public wxPanel
{
public:
	DashboardExpensesPanel(wxWindow *parent, wxWindowID id);

	void SetExpenses(std::vector<StringValue> expenses);
	void Update();

private:
	std::vector<StringValue> expenses;
	float totalValue;
	float maxValue;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};
