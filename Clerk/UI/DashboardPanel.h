#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "DataPanel.h"

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
	std::vector<DashboardAccountBalance> virtualBalance;
	std::vector<DashboardAccountBalance> expenses;	
	std::vector<DahboardBudget> budgets;
	std::vector<DahboardBudget> credits;
	std::vector<DahboardBudget> goals;
	wxString totalBalance;
	wxString totalExpenses;

	int paddingX;
	int paddingY;

	wxColor color0;
	wxColor color50;
	wxColor color100;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};

