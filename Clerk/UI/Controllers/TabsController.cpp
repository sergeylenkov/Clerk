#include "TabsController.h"

using namespace Clerk::UI;

TabsController::TabsController(DataContext& context, Icons& icons) : _context(context), _icons(icons) {
	_panel = nullptr;
}

void TabsController::SetTabsPanel(TabsPanel* panel) {
	_panel = panel;
}

void TabsController::RestoreLastTabs() {
	for (auto& tab : Settings::GetInstance().GetTabs()) {
		TabType type = static_cast<TabType>(tab.type);
		OpenNewTab(type);
	}
}

void TabsController::OpenNewTab(TabType type) {
	if (_panel) {		
		DataPanel* tabPanel = CreatePanel(type);		
		wxString title = GetTabTitle(type);

		if (tabPanel) {
			tabPanel->type = type;
			_panel->AddPanel(tabPanel, title);
		}
	}
}

DataPanel* TabsController::CreatePanel(TabType type) {
	switch (type)
	{
		case TabType::Dashboard:
			return new DashboardPanel(_panel, _context);
			break;
		case TabType::Transactions:
			return new TransactionsListPanel(_panel, _context);
			break;
		case TabType::Reports:
			return new DashboardPanel(_panel, _context);
			break;
		case TabType::Budgets:
			return new BudgetsPanel(_panel, _context);
			break;
		case TabType::Schedulers:
			return new SchedulersPanel(_panel, _context);
			break;
		case TabType::Goals:
			return new GoalsPanel(_panel, _context);
			break;
		case TabType::Tags:
			return new TagsPanel(_panel, _context);
			break;
		case TabType::Alerts:
			return new AlertsPanel(_panel, _context);
			break;
		case TabType::Trash:
			return new TrashPanel(_panel, _context);
			break;
		default:
			break;
	}

	return nullptr;
}

wxString TabsController::GetTabTitle(TabType type) {
	switch(type)
	{
		case TabType::Dashboard:
			return "Dashboard";
			break;
		case TabType::Transactions:
			return "Transactions";
			break;
		case TabType::Reports:
			return "";
			break;
		case TabType::Budgets:
			return "Budgets";
			break;
		case TabType::Schedulers:
			return "Schedulers";
			break;
		case TabType::Goals:
			return "Goals";
			break;
		case TabType::Tags:
			return "Tags";
			break;
		case TabType::Alerts:
			return "Alerts";
			break;
		case TabType::Trash:
			return "Trash";
			break;
		default:
			break;
	}

	return "";
}