#pragma once

#include <wx/wx.h>
#include "../DataPanel.h"
#include "./DashboardSchedulersPanel.h"
#include "./DashboardBudgetsPanel.h"
#include "./DashboardExpensesPanel.h"
#include "./DashboardReceiptsPanel.h"
#include "./DashboardAccountsPanel.h"
#include "./DashboardBalancePanel.h"
#include "./DashboardGoalsPanel.h"
#include "./DashboardDebtsPanel.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;

class DashboardPanel : public DataPanel
{
public:
	DashboardPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~DashboardPanel();

	void Update();

private:
	wxScrolledWindow*_scrolledWindow;
	wxPanel* _leftPanel;
	wxPanel* _rightPanel;
	DashboardSchedulersPanel* _schedulersPanel;
	DashboardBudgetsPanel* _budgetsPanel;
	DashboardExpensesPanel* _expensesPanel;
	DashboardReceiptsPanel* _receiptsPanel;
	DashboardAccountsPanel* _accountsPanel;
	DashboardBalancePanel* _balancePanel;
	DashboardGoalsPanel* _goalsPanel;
	DashboardDebtsPanel* _debtsPanel;
	DashboardViewModel* _viewModel;
};

