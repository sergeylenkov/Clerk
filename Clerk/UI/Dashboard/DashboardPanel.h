#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../DataPanel.h"
#include "./DashboardSchedulersPanel.h"
#include "./DashboardBudgetsPanel.h"
#include "./DashboardExpensesPanel.h"
#include "./DashboardAccountsPanel.h"
#include "./DashboardBalancePanel.h"
#include "./DashboardGoalsPanel.h"
#include "./DashboardDebtsPanel.h"

class DashboardPanel : public DataPanel
{
public:
	DashboardPanel(wxWindow *parent, wxWindowID id);	

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
	DashboardDebtsPanel *debtsPanel;
};

