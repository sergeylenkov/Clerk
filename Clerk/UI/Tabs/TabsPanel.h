#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include "../../Defines.h"
#include "../../Data/Settings.h"
#include "../../Data/DataContext.h"
#include "../Transactions/TransactionsListPanel.h"
#include "../Dashboard/DashboardPanel.h"
#include "../Reports/ReportExpensesByMonthPanel.h"
#include "../Reports/ReportBalanceByMonthPanel.h"
#include "../Reports/ReportExpensesForPeriodPanel.h"
#include "../Budgets/BudgetsPanel.h"
#include "../TrashPanel.h"
#include "../Schedulers/SchedulersPanel.h"
#include "../Goals/GoalsPanel.h"
#include "../Alerts/AlertsPanel.h"
#include "../TagsPanel.h"
#include "../TreeMenu/Enums.h"
#include "../../Commands/CommandsInvoker.h"
#include "./TabsContextMenu.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

class TabsPanel : public wxPanel
{
public:
	TabsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker, Icons& icons);
	~TabsPanel();
		
	void AddPanel(DataPanel* panel, wxString title, int iconIndex);
	void SelectTab(int index);
	void SelectLastTab();

private:
	DataContext& _context;
	CommandsInvoker& _commandsInvoker;
	Icons& _icons;
	wxNotebook *_notebook;	
	std::vector<DataPanel *> _tabsPanels;

	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void RemoveTab(int index);
};

