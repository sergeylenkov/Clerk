#include <wx/wx.h>
#include "DataPanel.h"

#pragma once

struct DashboardAccountBalance
{
	wxString name;
	wxString value;
};

struct DahboardBudget
{
	wxString name;
	wxString amount;
	wxString currentAmount;
	wxString remainAmount;
	float percent;
};

class DashboardPanel : public DataPanel
{
public:
	DashboardPanel(wxWindow *parent, wxWindowID id);
	~DashboardPanel();

	void Update();

private:
	std::vector<DashboardAccountBalance> balance;
	std::vector<DashboardAccountBalance> expenses;
	std::vector<DahboardBudget> budgets;
	std::vector<DahboardBudget> credits;

	int paddingX;
	int paddingY;

	wxColor color0;
	wxColor color50;
	wxColor color100;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};

