#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class DashboardExpensesPanel : public wxPanel
{
public:
	DashboardExpensesPanel(wxWindow *parent, wxWindowID id);

	void SetExpenses(std::vector<AccountValue> expenses);
	void Update();

private:
	std::vector<AccountValue> expenses;
	CurrencyValue totalValue;
	float maxValue;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
