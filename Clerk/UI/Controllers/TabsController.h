#pragma once

#include <wx/wx.h>
#include <optional>
#include "../Tabs/TabsPanel.h"
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"
#include "../Dashboard/DashboardPanel.h"
#include "../Transactions/TransactionsListPanel.h"
#include "../Budgets/BudgetsPanel.h"
#include "../TrashPanel.h"
#include "../Schedulers/SchedulersListPanel.h"
#include "../Goals/GoalsPanel.h"
#include "../Alerts/AlertsListPanel.h"
#include "../TagsPanel.h"
#include "../Reports/ReportExpensesByMonthPanel.h"
#include "../Reports/ReportBalanceByMonthPanel.h"
#include "../Reports/ReportExpensesForPeriodPanel.h"
#include "../Reports/ReportReceiptsByMonthPanel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TabsController {
		public:
			TabsController(DataContext& context, Icons& icons);

			void SetTabsPanel(TabsPanel* panel);
			void OpenTab(TabType type);
			void OpenAccountTab(int id);
			void OpenAccountsTab(std::optional<AccountType> type);
			void OpenReportTab(int id);
			void RestoreLastTabs();

		private:
			TabsPanel* _tabsPanel;
			DataContext& _context;
			Icons& _icons;

			DataPanel* CreatePanel(TabType type);
			wxString GetTabTitle(TabType type);
			wxString GetTabTitleByAccountType(AccountType type);
			int GetIconIndex(TabType type);
			DataPanel* GetReportPanelById(int id);
		};
	}
}
