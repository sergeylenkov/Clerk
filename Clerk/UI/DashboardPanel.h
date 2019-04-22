#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "DataPanel.h"
#include "./Dashboard/DashboardSchedulersPanel.h"
#include "./Dashboard/DashboardBudgetsPanel.h"
#include "./Dashboard/DashboardExpensesPanel.h"
#include "./Dashboard/DashboardAccountsPanel.h"
#include "./Dashboard/DashboardBalancePanel.h"
#include "./Dashboard/DashboardGoalsPanel.h"

class DashboardPanel : public DataPanel
{
public:
	DashboardPanel(wxWindow *parent, wxWindowID id);
	~DashboardPanel();

	void Update();

private:
	wxScrolledWindow *scrolledWindow;
	wxPanel *leftPanel;
	wxPanel *rightPanel;
	DashboardSchedulersPanel *schedulersPanel;
	DashboardBudgetsPanel *budgetsPanel;
	DashboardExpensesPanel *expensesPanel;
	DashboardAccountsPanel *accountsPanel;
	DashboardBalancePanel *balancePanel;
	DashboardGoalsPanel *goalsPanel;
};

