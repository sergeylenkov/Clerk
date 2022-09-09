#include "TabsPanel.h"

TabsPanel::TabsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker) : wxPanel(parent), _context(context), _commandsInvoker(commandsInvoker) {
	_contextMenuTab = 0;

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	mainSizer->Add(_notebook, 1, wxEXPAND | wxALL, 0);	

	this->SetSizer(mainSizer);
	this->Layout();

	_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &TabsPanel::OnTabChanged, this);
	_notebook->Bind(wxEVT_RIGHT_DOWN, &TabsPanel::OnTabClick, this);
}

TabsPanel::~TabsPanel()
{
	Settings::GetInstance().ClearTabs();

	for (auto &panel : _tabsPanels)
	{
		Settings::GetInstance().AddTab(static_cast<int>(panel->type), panel->id);
	}

	Settings::GetInstance().SetSelectedTab(_notebook->GetSelection());

	delete _notebook;
}

void TabsPanel::AddPanel(DataPanel* panel, wxString title) {
	wxPanel* tabPanel = new wxPanel(_notebook);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	tabPanel->SetSizer(sizer);

	_tabs.push_back(tabPanel);
	_tabsSizer.push_back(sizer);
	_tabsPanels.push_back(panel);

	_notebook->AddPage(tabPanel, title);

	panel->Reparent(tabPanel);
	
	sizer->Add(panel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	panel->Update();
}

void TabsPanel::CreateTab(TreeMenuItemType type, std::shared_ptr<void> object) {
	wxPanel *panel = new wxPanel(_notebook);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(sizer);

	_tabs.push_back(panel);
	_tabsSizer.push_back(sizer);
	_tabsPanels.push_back(nullptr);

	_notebook->AddPage(panel, "");

	int index = _tabs.size() - 1;

	//CreatePanel(index, type, object);
}

void TabsPanel::AddTab(TreeMenuItemType type, std::shared_ptr<void> object) {
	CreateTab(type, object);
	_notebook->ChangeSelection(_tabs.size() - 1);
}

void TabsPanel::UpdateCurrentTab(TreeMenuItemType type, std::shared_ptr<void> object) {
	int i = _notebook->GetSelection();
	//CreatePanel(i, type, object);
}

/*void TabsPanel::RestoreTabs() {
	for (auto &tab : Settings::GetInstance().GetTabs()) {
		TreeMenuItemType type = (TreeMenuItemType)tab.type;

		if (type == TreeMenuItemType::Account) {
			std::shared_ptr<AccountViewModel> tabAccount = _context.GetAccountsService().GetById(tab.id);

			if (tabAccount) {
				CreateTab(type, tabAccount);
			}
		} else if (type == TreeMenuItemType::Report) {
			std::shared_ptr<ReportViewModel> tabReport = _context.GetReportsService().GetById(tab.id);

			if (tabReport) {
				CreateTab(type, tabReport);
			}
		}
		else {
			CreateTab(type, nullptr);
		}
	}

	int index = Settings::GetInstance().GetSelectedTab();
	_notebook->ChangeSelection(index);
}*/

bool TabsPanel::IsTabExists(TreeMenuItemType type, int id) {
	/*bool found = false;

	for (auto &tabPanel : _tabsPanels) {
		if (type == TreeMenuItemType::Accounts) {
			if (tabPanel->type == type && tabPanel->id == id) {
				return true;
			}
		}
		else if (type == TreeMenuItemType::Reports) {
			if (tabPanel->type == type && tabPanel->id == id) {
				return true;
			}
		}
		else if (tabPanel->type == type) {
			return true;
		}
	}*/

	return false;
}

void TabsPanel::SelectTab(TreeMenuItemType type, int id) {
	/*for (unsigned int i = 0; i < _tabsPanels.size(); i++) {
		DataPanel *tabPanel = _tabsPanels[i];

		if (type == TreeMenuItemType::Account) {
			if (tabPanel->type == type && tabPanel->id == id) {
				_notebook->ChangeSelection(i);
			}
		}
		else if (type == TreeMenuItemType::Reports) {
			if (tabPanel->type == type && tabPanel->id == id) {
				_notebook->ChangeSelection(i);
			}
		}
		else if (tabPanel->type == type) {
			_notebook->ChangeSelection(i);
		}
	}*/
}

//void TabsPanel::CreatePanel(int tabIndex, TreeMenuItemType type, std::shared_ptr<void> object) {
//	if (type == TreeMenuItemType::Account) {
//		auto account = std::static_pointer_cast<AccountViewModel>(object);
//		CreateAccountPanel(tabIndex, account);
//	}
//	else if (type == TreeMenuItemType::Accounts || type == TreeMenuItemType::Receipts
//		|| type == TreeMenuItemType::Expenses || type == TreeMenuItemType::Deposits || type == TreeMenuItemType::Virtual) {
//		CreateAccountsPanel(tabIndex, (TreeMenuItemType)type);
//	}
//	else if (type == TreeMenuItemType::Dashboard) {
//		CreateDashboardPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Budgets) {
//		CreateBudgetsPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Schedulers) {
//		CreateSchedulersPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Goals) {
//		CreateGoalsPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Trash) {
//		CreateTrashPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Tags) {
//		CreateTagsPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Alerts) {
//		CreateAlertsPanel(tabIndex);
//	}
//	else if (type == TreeMenuItemType::Report) {
//		auto report = std::static_pointer_cast<ReportViewModel>(object);
//		CreateReportPanel(tabIndex, report);
//	}
//}
//
//void TabsPanel::CreateAccountPanel(int tabIndex, std::shared_ptr<AccountViewModel> account) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, _context);
//
//	transactionList->OnAdd = std::bind(&TabsPanel::AddTransaction, this);
//	transactionList->OnCopy = std::bind(&TabsPanel::CopyTransaction, this, std::placeholders::_1);
//	transactionList->OnEdit = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
//	transactionList->OnSplit = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = transactionList;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Account;
//	_tabsPanels[tabIndex]->id = account->id;
//
//	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, account->name);
//	UpdateTransactionList(transactionList, TreeMenuItemType::Account, account);
//}
//
//void TabsPanel::CreateAccountsPanel(int tabIndex, TreeMenuItemType type) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, _context);
//
//	transactionList->OnAdd = std::bind(&TabsPanel::AddTransaction, this);
//	transactionList->OnCopy = std::bind(&TabsPanel::CopyTransaction, this, std::placeholders::_1);
//	transactionList->OnEdit = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
//	transactionList->OnSplit = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = transactionList;
//	_tabsPanels[tabIndex]->type = type;
//
//	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	wxString name = wxT("");
//
//	if (type == TreeMenuItemType::Deposits) {
//		name = wxT("Deposits");
//	}
//	else if (type == TreeMenuItemType::Expenses) {
//		name = wxT("Expenses");
//	}
//	else if (type == TreeMenuItemType::Receipts) {
//		name = wxT("Receipts");
//	}
//	else if (type == TreeMenuItemType::Accounts) {
//		name = wxT("Transactions");
//	}
//
//	_notebook->SetPageText(tabIndex, name);
//	UpdateTransactionList(transactionList, type, nullptr);
//}
//
//void TabsPanel::CreateDashboardPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	DashboardPanel *dashboardPanel = new DashboardPanel(panel, _context);
//
//	_tabsPanels[tabIndex] = dashboardPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Dashboard;
//
//	sizer->Add(dashboardPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Dashboard"));
//
//	dashboardPanel->Update();
//}
//
//void TabsPanel::CreateBudgetsPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	BudgetsPanel *budgetPanel = new BudgetsPanel(panel, _context);
//
//	budgetPanel->OnAdd = std::bind(&TabsPanel::AddBudget, this);
//	budgetPanel->OnEdit = std::bind(&TabsPanel::EditBudget, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = budgetPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Budgets;
//
//	sizer->Add(budgetPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Budgets"));
//
//	budgetPanel->Update();
//}
//
//void TabsPanel::CreateSchedulersPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	SchedulersPanel *schedulersPanel = new SchedulersPanel(panel, _context);
//
//	schedulersPanel->OnAdd = std::bind(&TabsPanel::AddScheduler, this);
//	schedulersPanel->OnEdit = std::bind(&TabsPanel::EditScheduler, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = schedulersPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Schedulers;
//
//	sizer->Add(schedulersPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Schedulers"));
//
//	schedulersPanel->Update();
//}
//
//void TabsPanel::CreateGoalsPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	GoalsPanel *goalsPanel = new GoalsPanel(panel, _context);
//
//	goalsPanel->OnAdd = std::bind(&TabsPanel::AddGoal, this);
//	goalsPanel->OnEdit = std::bind(&TabsPanel::EditGoal, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = goalsPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Goals;
//
//	sizer->Add(goalsPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Goals"));
//
//	goalsPanel->Update();
//}
//
//void TabsPanel::CreateReportPanel(int tabIndex, std::shared_ptr<ReportViewModel> report) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	DataPanel *reportPanel;
//
//	if (report->id == 1) {
//		reportPanel = new ReportExpensesByMonthPanel(panel, _context);
//	}
//	else if (report->id == 2) {
//		reportPanel = new ReportBalancePanel(panel, _context);
//	}
//	else {
//		reportPanel = new ReportExpensesForPeriodPanel(panel, _context);
//	}
//
//	_tabsPanels[tabIndex] = reportPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Report;
//	_tabsPanels[tabIndex]->id = report->id;
//
//	sizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, report->name);
//
//	reportPanel->Update();
//}
//
//void TabsPanel::CreateTrashPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	TrashPanel *trashPanel = new TrashPanel(panel, _context);
//
//	_tabsPanels[tabIndex] = trashPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Trash;
//
//	sizer->Add(trashPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Trash"));
//
//	trashPanel->Update();
//}
//
//void TabsPanel::CreateTagsPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	TagsPanel *tagsPanel = new TagsPanel(panel, _context);
//
//	_tabsPanels[tabIndex] = tagsPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Tags;
//
//	sizer->Add(tagsPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Tags"));
//
//	tagsPanel->Update();
//}
//
//void TabsPanel::CreateAlertsPanel(int tabIndex) {
//	wxPanel *panel = _tabs[tabIndex];
//	wxBoxSizer *sizer = _tabsSizer[tabIndex];
//	DataPanel *currentPanel = _tabsPanels[tabIndex];
//
//	if (currentPanel) {
//		currentPanel->Destroy();
//	}
//
//	AlertsPanel *alertsPanel = new AlertsPanel(panel, _context);
//
//	alertsPanel->OnAdd = std::bind(&TabsPanel::AddAlert, this);
//	alertsPanel->OnEdit = std::bind(&TabsPanel::EditAlert, this, std::placeholders::_1);
//
//	_tabsPanels[tabIndex] = alertsPanel;
//	_tabsPanels[tabIndex]->type = TreeMenuItemType::Alerts;
//
//	sizer->Add(alertsPanel, 1, wxEXPAND | wxALL, 0);
//	sizer->Layout();
//
//	_notebook->SetPageText(tabIndex, wxT("Alerts"));
//
//	alertsPanel->Update();
//}


void TabsPanel::OnTabChanged(wxBookCtrlEvent &event) {
	//
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	_contextMenuTab = _notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	wxMenu *menu = new wxMenu();

	wxMenuItem *leftItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::MoveLeft), wxT("Move to Left"));
	wxMenuItem *rightItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::MoveRight), wxT("Move to Right"));
	wxMenuItem *closeItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Close), wxT("Close"));

	if (_notebook->GetPageCount() == 1) {
		leftItem->Enable(false);
		leftItem->SetTextColour(*wxLIGHT_GREY);

		rightItem->Enable(false);
		rightItem->SetTextColour(*wxLIGHT_GREY);

		closeItem->Enable(false);
		closeItem->SetTextColour(*wxLIGHT_GREY);
	}

	if (_contextMenuTab == 0) {
		leftItem->Enable(false);
		leftItem->SetTextColour(*wxLIGHT_GREY);
	}

	if (_contextMenuTab == _notebook->GetPageCount() - 1) {
		rightItem->Enable(false);
		rightItem->SetTextColour(*wxLIGHT_GREY);
	}

	menu->Append(leftItem);
	menu->Append(rightItem);
	menu->AppendSeparator();
	menu->Append(closeItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsPanel::OnTabMenuClose, this, static_cast<int>(ContextMenuTypes::Close));

	_notebook->PopupMenu(menu, point);

	delete menu;

	event.Skip();
}

void TabsPanel::OnTabMenuClose(wxCommandEvent &event) {	
	RemoveTab(_contextMenuTab);
}

/*void TabsPanel::Update() {
	for (unsigned int i = 0; i < _tabsPanels.size(); i++) {
		_tabsPanels[i]->Update();
	}
}*/

/*void TabsPanel::AddTransaction() {
	if (OnAddTransaction) {
		OnAddTransaction();
	}
}

void TabsPanel::CopyTransaction(std::shared_ptr<TransactionViewModel> transaction) {
	if (OnCopyTransaction) {
		OnCopyTransaction(transaction);
	}
}

void TabsPanel::EditTransaction(std::shared_ptr<TransactionViewModel> transaction) {
	if (OnEditTransaction) {
		OnEditTransaction(transaction);
	}
}

void TabsPanel::SplitTransaction(std::shared_ptr<TransactionViewModel> transaction) {
	if (OnSplitTransaction) {
		OnSplitTransaction(transaction);
	}
}*/

/*void TabsPanel::UpdateTransactionList(TransactionsListPanel* transactionList, TreeMenuItemType type, std::shared_ptr<AccountViewModel> account)
{
	transactionList->Update();

	if (type == TreeMenuItemTypes::Account) {
		transactionList->SetType(type);
		transactionList->SetAccount(account);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::Expenses) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::Receipts) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::Deposits) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::Accounts) {
		transactionList->SetType(type);
		transactionList->Update();
	}
}*/

/*std::shared_ptr<TransactionViewModel> TabsPanel::GetSelectedTransaction() {
	int i = _notebook->GetSelection();
	DataPanel *currentPanel = _tabsPanels[i];	

	if (currentPanel->type == TreeMenuItemType::Account) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		//return transactionList->GetTransaction();
	}

	return nullptr;
}*/

void TabsPanel::RemoveTab(int index) {
	DataPanel *currentPanel = _tabsPanels[index];

	_notebook->RemovePage(index);

	_tabs.erase(_tabs.begin() + index);
	_tabsSizer.erase(_tabsSizer.begin() + index);
	_tabsPanels.erase(_tabsPanels.begin() + index);
	
	delete currentPanel;
}

/*void TabsPanel::AddBudget() {
	if (OnAddBudget) {
		OnAddBudget();
	}
}

void TabsPanel::EditBudget(std::shared_ptr<BudgetViewModel> budget) {
	if (OnEditBudget) {
		OnEditBudget(budget);
	}
}

void TabsPanel::AddScheduler() {
	if (OnAddScheduler) {
		OnAddScheduler();
	}
}

void TabsPanel::EditScheduler(std::shared_ptr<SchedulerViewModel> scheduler) {
	if (OnEditScheduler) {
		OnEditScheduler(scheduler);
	}
}

void TabsPanel::AddGoal() {
	if (OnAddGoal) {
		OnAddGoal();
	}
}

void TabsPanel::EditGoal(std::shared_ptr<GoalViewModel> goal) {
	if (OnEditGoal) {
		OnEditGoal(goal);
	}
}


void TabsPanel::AddAlert() {
	if (OnAddAlert) {
		OnAddAlert();
	}
}

void TabsPanel::EditAlert(std::shared_ptr<AlertViewModel> alert) {
	if (OnEditAlert) {
		OnEditAlert(alert);
	}
}*/