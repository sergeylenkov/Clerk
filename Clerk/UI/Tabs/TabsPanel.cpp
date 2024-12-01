#include "TabsPanel.h"

TabsPanel::TabsPanel(wxWindow *parent, DataContext& context, Icons& icons):
	wxPanel(parent),
	_context(context),
	_icons(icons)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	_notebook->SetImageList(_icons.GetImageList());

	mainSizer->Add(_notebook, 1, wxEXPAND);	

	SetSizer(mainSizer);
	Layout();

	_notebook->Bind(wxEVT_RIGHT_DOWN, &TabsPanel::OnTabClick, this);

	RestoreLastTabs();
}

TabsPanel::~TabsPanel()
{
	Settings::GetInstance().ClearTabs();

	for (auto& panel : _tabsPanels)
	{
		Settings::GetInstance().AddTab(static_cast<int>(panel->type), panel->id);
	}

	Settings::GetInstance().SetSelectedTab(_notebook->GetSelection());

	delete _notebook;
}

void TabsPanel::AddPanel(DataPanel* dataPanel, wxString title, int iconIndex) {
	wxPanel* tabPanel = new wxPanel(_notebook);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	tabPanel->SetSizer(sizer);
	dataPanel->Reparent(tabPanel);
	sizer->Add(dataPanel, 1, wxEXPAND);

	_notebook->AddPage(tabPanel, title, true, iconIndex);
	
	sizer->Layout();

	_tabsPanels.push_back(dataPanel);
}

void TabsPanel::SelectTab(int index) {
	if (index < _notebook->GetPageCount()) {
		_notebook->SetSelection(index);
	}
}

void TabsPanel::SelectLastTab() {
	_notebook->SetSelection(_notebook->GetPageCount() - 1);
}

void TabsPanel::RemoveTab(int index) {
	DataPanel* currentPanel = _tabsPanels[index];

	_notebook->RemovePage(index);

	_tabsPanels.erase(_tabsPanels.begin() + index);

	delete currentPanel;
}

void TabsPanel::RestoreLastTabs() {
	for (auto& tab : Settings::GetInstance().GetTabs()) {
		TabType type = static_cast<TabType>(tab.type);

		if (type == TabType::Transactions || type == TabType::Deposits || type == TabType::Receipts
			|| type == TabType::Expenses || type == TabType::Debts || type == TabType::Virtual) {
			if (tab.id != -1) {
				OpenAccountTab(tab.id);
			}
			else {
				switch (type)
				{
				case TabType::Deposits:
					OpenAccountsTab(AccountType::Deposit);
					break;
				case TabType::Receipts:
					OpenAccountsTab(AccountType::Receipt);
					break;
				case TabType::Expenses:
					OpenAccountsTab(AccountType::Expense);
					break;
				case TabType::Debts:
					OpenAccountsTab(AccountType::Debt);
					break;
				case TabType::Virtual:
					OpenAccountsTab(AccountType::Virtual);
					break;
				default:
					OpenAccountsTab(nullopt);
					break;
				}
			}
		}		
		else if (type == TabType::Reports && tab.id != -1) {
			OpenReportTab(tab.id);
		}
		else {
			OpenTab(type);
		}
	}

	SelectTab(Settings::GetInstance().GetSelectedTab());
}

void TabsPanel::OpenTab(TabType type) {
	DataPanel* tabPanel = CreatePanel(type);

	if (tabPanel) {
		tabPanel->id = -1;
		tabPanel->type = type;

		AddPanel(tabPanel, GetTabTitle(type), GetIconIndex(type));
		SelectLastTab();
	}
}

void TabsPanel::OpenAccountTab(int id) {
	TransactionsListPanel* tabPanel = static_cast<TransactionsListPanel*>(CreatePanel(TabType::Transactions));

	auto account = _context.GetAccountsService().GetById(id);

	if (tabPanel && account) {
		tabPanel->id = id;
		tabPanel->type = TabType::Transactions;

		tabPanel->SetAccount(account);
		tabPanel->Update();

		AddPanel(tabPanel, account->name, _icons.GetIconIndexForAccount(account->icon));
		SelectLastTab();
	}
}

void TabsPanel::OpenAccountsTab(std::optional<AccountType> type) {
	TransactionsListPanel* tabPanel = static_cast<TransactionsListPanel*>(CreatePanel(TabType::Transactions));

	if (tabPanel) {
		tabPanel->id = -1;
		tabPanel->type = GetTabTypeByAccountType(type.value());

		wxString title = GetTabTitle(tabPanel->type);

		if (type.has_value()) {
			tabPanel->SetAccountType(type.value());
			tabPanel->Update();

			AddPanel(tabPanel, title, GetIconIndex(tabPanel->type));
		}
		else {
			tabPanel->Update();
			AddPanel(tabPanel, title, GetIconIndex(tabPanel->type));
		}

		SelectLastTab();
	}
}

void TabsPanel::OpenReportTab(int id) {
	DataPanel* tabPanel = GetReportPanelById(id);

	auto report = _context.GetReportsService().GetById(id);

	if (tabPanel && report) {
		tabPanel->id = id;
		tabPanel->type = TabType::Reports;

		tabPanel->Update();

		AddPanel(tabPanel, report->name, GetIconIndex(TabType::Reports));
		SelectLastTab();
	}
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	unsigned int tabId = _notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	bool isCloseEnable = _notebook->GetPageCount() > 1;
	bool isLeftEnable = isCloseEnable && tabId > 0;
	bool isRightEnable = isCloseEnable && tabId < _notebook->GetPageCount() - 1;

	TabsContextMenu* menu = new TabsContextMenu(_icons, tabId, isLeftEnable, isRightEnable, isCloseEnable);

	menu->OnClose = [&](int tabId) {
		RemoveTab(tabId);
	};

	menu->OnMoveLeft = [&](int tabId) {
		
	};

	menu->OnMoveRight = [&](int tabId) {

	};

	_notebook->PopupMenu(menu, point);

	delete menu;

	event.Skip();
}

DataPanel* TabsPanel::CreatePanel(TabType type) {
	switch (type)
	{
	case TabType::Dashboard:
		return new DashboardPanel(this, _context, _icons);
		break;
	case TabType::Transactions:
		return new TransactionsListPanel(this, _context, _icons);
		break;	
	case TabType::Budgets:
		return new BudgetsListPanel(this, _context, _icons);
		break;
	case TabType::Schedulers:
		return new SchedulersListPanel(this, _context, _icons);
		break;
	case TabType::Goals:
		return new GoalsListPanel(this, _context, _icons);
		break;
	case TabType::Tags:
		return new TagsPanel(this, _context, _icons);
		break;
	case TabType::Alerts:
		return new AlertsListPanel(this, _context, _icons);
		break;
	case TabType::Trash:
		return new TrashPanel(this, _context, _icons);
		break;
	default:
		break;
	}

	return nullptr;
}

wxString TabsPanel::GetTabTitle(TabType type) {
	switch (type)
	{
	case TabType::Dashboard:
		return _("Dashboard");
		break;
	case TabType::Transactions:
		return _("Transactions");
		break;
	case TabType::Deposits:
		return _("Deposits");
		break;
	case TabType::Receipts:
		return _("Receipts");
		break;
	case TabType::Expenses:
		return _("Expenses");
		break;
	case TabType::Debts:
		return _("Debts");
		break;
	case TabType::Virtual:
		return _("Virtual");
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

int TabsPanel::GetIconIndex(TabType type) {
	switch (type)
	{
	case TabType::Dashboard:
		return 0;
		break;
	case TabType::Transactions:
		return 1;
		break;
	case TabType::Deposits:
		return 2;
		break;
	case TabType::Receipts:
		return 2;
		break;
	case TabType::Expenses:
		return 2;
		break;
	case TabType::Debts:
		return 2;
		break;
	case TabType::Reports:
		return 5;
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

DataPanel* TabsPanel::GetReportPanelById(int id) {
	switch (id)
	{
	case 1:
		return new ReportExpensesByMonthPanel(this, _context, _icons);
	case 2:
		return new ReportExpensesForPeriodPanel(this, _context, _icons);
	case 3:
		return new ReportBalanceByMonthPanel(this, _context, _icons);
	case 4:
		return new ReportReceiptsByMonthPanel(this, _context, _icons);
	case 5:
		return new ReportReceiptsAndExpensesByMonthPanel(this, _context, _icons);
	default:
		break;
	}

	return nullptr;
}

TabType TabsPanel::GetTabTypeByAccountType(AccountType type) {
	switch (type)
	{
	case AccountType::Deposit:
		return TabType::Deposits;
		break;
	case AccountType::Receipt:
		return TabType::Receipts;
		break;
	case AccountType::Expense:
		return TabType::Expenses;
		break;
	case AccountType::Debt:
		return TabType::Debts;
		break;
	case AccountType::Virtual:
		return TabType::Virtual;
		break;
	default:
		break;
	}

	return TabType::Transactions;
}