#include "TabsController.h"

using namespace Clerk::UI;

TabsController::TabsController(DataContext& context, Icons& icons) : _context(context), _icons(icons) {
	_tabsPanel = nullptr;
}

void TabsController::SetTabsPanel(TabsPanel* panel) {
	_tabsPanel = panel;
}

void TabsController::RestoreLastTabs() {
	for (auto& tab : Settings::GetInstance().GetTabs()) {
		TabType type = static_cast<TabType>(tab.type);

		if (type == TabType::Transactions && tab.id != -1) {
			OpenAccountTab(tab.id);
		} else {
			OpenTab(type);
		}		
	}

	_tabsPanel->SelectTab(Settings::GetInstance().GetSelectedTab());
}

void TabsController::OpenTab(TabType type) {
	if (_tabsPanel) {
		DataPanel* tabPanel = CreatePanel(type);

		if (tabPanel) {
			tabPanel->id = -1;
			tabPanel->type = type;

			_tabsPanel->AddPanel(tabPanel, GetTabTitle(type), GetIconIndex(type));
			_tabsPanel->SelectLastTab();
		}
	}
}

void TabsController::OpenAccountTab(int id) {
	if (_tabsPanel) {
		TransactionsListPanel* tabPanel = static_cast<TransactionsListPanel*>(CreatePanel(TabType::Transactions));

		auto account = _context.GetAccountsService().GetById(id);

		if (tabPanel && account) {
			tabPanel->id = id;
			tabPanel->type = TabType::Transactions;

			tabPanel->SetAccount(account);
			tabPanel->Update();

			_tabsPanel->AddPanel(tabPanel, account->name, _icons.GetIconIndexForAccount(account->icon));
			_tabsPanel->SelectLastTab();
		}
	}
}

void TabsController::OpenAccountsTab(std::optional<AccountType> type) {
	if (_tabsPanel) {
		TransactionsListPanel* tabPanel = static_cast<TransactionsListPanel*>(CreatePanel(TabType::Transactions));

		if (tabPanel) {
			tabPanel->id = -1;
			tabPanel->type = TabType::Transactions;

			if (type.has_value()) {
				tabPanel->SetAccountType(type.value());
				tabPanel->Update();

				_tabsPanel->AddPanel(tabPanel, GetTabTitleByAccountType(type.value()), GetIconIndex(TabType::Transactions));
			}
			else {
				tabPanel->Update();
				_tabsPanel->AddPanel(tabPanel, _("Transactions"), GetIconIndex(TabType::Transactions));
			}

			_tabsPanel->SelectLastTab();
		}
	}
}

void TabsController::OpenReportTab(int id) {
	if (_tabsPanel) {
		ReportExpensesByMonthPanel* tabPanel = new ReportExpensesByMonthPanel(_tabsPanel, _context);

		auto report = _context.GetReportsService().GetById(id);

		if (tabPanel && report) {
			tabPanel->id = id;
			tabPanel->type = TabType::Reports;
			
			tabPanel->Update();

			_tabsPanel->AddPanel(tabPanel, report->name, GetIconIndex(TabType::Reports));
			_tabsPanel->SelectLastTab();
		}
	}
}

DataPanel* TabsController::CreatePanel(TabType type) {
	switch (type)
	{
		case TabType::Dashboard:
			return new DashboardPanel(_tabsPanel, _context);
			break;
		case TabType::Transactions:
			return new TransactionsListPanel(_tabsPanel, _context);
			break;
		case TabType::Budgets:
			return new BudgetsPanel(_tabsPanel, _context);
			break;
		case TabType::Schedulers:
			return new SchedulersPanel(_tabsPanel, _context);
			break;
		case TabType::Goals:
			return new GoalsPanel(_tabsPanel, _context);
			break;
		case TabType::Tags:
			return new TagsPanel(_tabsPanel, _context);
			break;
		case TabType::Alerts:
			return new AlertsPanel(_tabsPanel, _context);
			break;
		case TabType::Trash:
			return new TrashPanel(_tabsPanel, _context);
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
			return _("Dashboard");
			break;
		case TabType::Transactions:
			return _("Transactions");
			break;
		case TabType::Budgets:
			return _("Budgets");
			break;
		case TabType::Schedulers:
			return _("Schedulers");
			break;
		case TabType::Goals:
			return _("Goals");
			break;
		case TabType::Tags:
			return _("Tags");
			break;
		case TabType::Alerts:
			return _("Alerts");
			break;
		case TabType::Trash:
			return _("Trash");
			break;
		default:
			break;
	}

	return "";
}

wxString TabsController::GetTabTitleByAccountType(AccountType type) {
	switch (type)
	{
	case AccountType::Deposit:
		return _("Deposits");
		break;
	case AccountType::Receipt:
		return _("Receipts");
		break;
	case AccountType::Expens:
		return _("Expenses");
		break;
	case AccountType::Debt:
		return _("Debts");
		break;
	case AccountType::Virtual:
		return _("Virtuals");
		break;
	default:
		break;
	}

	return "";
}

int TabsController::GetIconIndex(TabType type) {
	switch (type)
	{
		case TabType::Dashboard:
			return 0;
			break;
		case TabType::Transactions:
			return 1;
			break;
		case TabType::Reports:
			return 4;
			break;
		case TabType::Budgets:
			return 6;
			break;
		case TabType::Schedulers:
			return 7;
			break;
		case TabType::Goals:
			return 8;
			break;
		case TabType::Tags:
			return 9;
			break;
		case TabType::Alerts:
			return 12;
			break;
		case TabType::Trash: {
			return _context.GetTransactionsService().GetDeleted().size() == 0 ? 11 : 10;
		}		
		default:
			break;
		}

	return 0;
}