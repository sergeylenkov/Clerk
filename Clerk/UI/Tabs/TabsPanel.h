#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include <optional>
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"
#include "../../Utils/Settings/Settings.h"
#include "../DataPanel/DataPanel.h"
#include "../Dashboard/DashboardPanel.h"
#include "../Transactions/TransactionsListPanel.h"
#include "../Budgets/BudgetsListPanel.h"
#include "../TrashPanel.h"
#include "../Schedulers/SchedulersListPanel.h"
#include "../Goals/GoalsListPanel.h"
#include "../Alerts/AlertsListPanel.h"
#include "../TagsPanel.h"
#include "../Reports/ReportExpensesByMonthPanel.h"
#include "../Reports/ReportBalanceByMonthPanel.h"
#include "../Reports/ReportExpensesForPeriodPanel.h"
#include "../Reports/ReportReceiptsByMonthPanel.h"
#include "../Reports/ReportReceiptsAndExpensesByMonthPanel.h"
#include "Enums.h"
#include "TabsContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

class TabsPanel : public wxPanel
{
public:
	TabsPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~TabsPanel();
		
	void AddPanel(DataPanel* panel, wxString title, int iconIndex);
	void SelectTab(int index);
	void SelectLastTab();
	void OpenTab(TabType type);
	void OpenAccountTab(int id);
	void OpenAccountsTab(std::optional<AccountType> type);
	void OpenReportTab(int id);
	void RestoreLastTabs();

private:
	DataContext& _context;
	Icons& _icons;
	wxNotebook *_notebook;	
	std::vector<DataPanel*> _tabsPanels;

	DataPanel* CreatePanel(TabType type);
	wxString GetTabTitle(TabType type);
	wxString GetTabTitleByAccountType(AccountType type);
	int GetIconIndex(TabType type);
	DataPanel* GetReportPanelById(int id);
	void OnTabClick(wxMouseEvent &event);
	void RemoveTab(int index);
};

