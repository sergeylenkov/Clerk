#include "TabsPanel.h"

TabsPanel::TabsPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 0);	

	this->SetSizer(mainSizer);
	this->Layout();

	notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &TabsPanel::OnTabChanged, this);
	notebook->Bind(wxEVT_RIGHT_DOWN, &TabsPanel::OnTabClick, this);
}

TabsPanel::~TabsPanel()
{
	Settings::GetInstance().ClearTabs();

	for (auto panel : tabsPanels)
	{
		Settings::GetInstance().AddTab(static_cast<int>(panel->type), panel->id);
	}

	Settings::GetInstance().SetSelectedTab(notebook->GetSelection());

	delete notebook;
}

void TabsPanel::CreateTab(TreeMenuItemTypes type, shared_ptr<void> object) {
	wxPanel *panel = new wxPanel(notebook);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(sizer);

	tabs.push_back(panel);
	tabsSizer.push_back(sizer);
	tabsPanels.push_back(nullptr);

	notebook->AddPage(panel, "");

	int index = tabs.size() - 1;

	CreatePanel(index, type, object);
}

void TabsPanel::AddTab(TreeMenuItemTypes type, shared_ptr<void> object) {
	CreateTab(type, object);
	notebook->ChangeSelection(tabs.size() - 1);
}

void TabsPanel::UpdateCurrentTab(TreeMenuItemTypes type, shared_ptr<void> object) {
	int i = notebook->GetSelection();
	CreatePanel(i, type, object);
}

void TabsPanel::RestoreTabs() {
	for (auto tab : Settings::GetInstance().GetTabs()) {
		TreeMenuItemTypes type = (TreeMenuItemTypes)tab.type;

		if (type == TreeMenuItemTypes::Account) {
			std::shared_ptr<Account> tabAccount = DataHelper::GetInstance().GetAccountById(tab.id);

			if (tabAccount) {
				CreateTab(type, tabAccount);
			}
		} else if (type == TreeMenuItemTypes::Report) {
			std::shared_ptr<Report> tabReport = DataHelper::GetInstance().GetReportById(tab.id);

			if (tabReport) {
				CreateTab(type, tabReport);
			}
		}
		else {
			CreateTab(type, nullptr);
		}
	}

	int index = Settings::GetInstance().GetSelectedTab();
	notebook->ChangeSelection(index);
}

bool TabsPanel::IsTabExists(TreeMenuItemTypes type, int id) {
	bool found = false;

	for (auto tabPanel : tabsPanels) {
		if (type == TreeMenuItemTypes::Account) {
			if (tabPanel->type == type && tabPanel->id == id) {
				return true;
			}
		}
		else if (type == TreeMenuItemTypes::Reports) {
			if (tabPanel->type == type && tabPanel->id == id) {
				return true;
			}
		}
		else if (tabPanel->type == type) {
			return true;
		}
	}

	return false;
}

void TabsPanel::SelectTab(TreeMenuItemTypes type, int id) {
	for (unsigned int i = 0; i < tabsPanels.size(); i++) {
		DataPanel *tabPanel = tabsPanels[i];

		if (type == TreeMenuItemTypes::Account) {
			if (tabPanel->type == type && tabPanel->id == id) {
				notebook->ChangeSelection(i);
			}
		}
		else if (type == TreeMenuItemTypes::Reports) {
			if (tabPanel->type == type && tabPanel->id == id) {
				notebook->ChangeSelection(i);
			}
		}
		else if (tabPanel->type == type) {
			notebook->ChangeSelection(i);
		}
	}
}

void TabsPanel::CreatePanel(int tabIndex, TreeMenuItemTypes type, shared_ptr<void> object) {
	if (type == TreeMenuItemTypes::Account) {
		auto account = std::static_pointer_cast<Account>(object);
		CreateAccountPanel(tabIndex, account);
	}
	else if (type == TreeMenuItemTypes::Accounts || type == TreeMenuItemTypes::Receipts 
		|| type == TreeMenuItemTypes::Expenses || type == TreeMenuItemTypes::Deposits || type == TreeMenuItemTypes::Virtual) {
		CreateAccountsPanel(tabIndex, (TreeMenuItemTypes)type);
	}
	else if (type == TreeMenuItemTypes::Dashboard) {
		CreateDashboardPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Budgets) {
		CreateBudgetsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Schedulers) {
		CreateSchedulersPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Goals) {
		CreateGoalsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Trash) {
		CreateTrashPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Tags) {
		CreateTagsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Alerts) {
		CreateAlertsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::Report) {
		auto report = std::static_pointer_cast<Report>(object);
		CreateReportPanel(tabIndex, report);
	}
}

void TabsPanel::CreateAccountPanel(int tabIndex, std::shared_ptr<Account> account) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, wxID_ANY);

	transactionList->OnAdd = std::bind(&TabsPanel::AddTransaction, this);
	transactionList->OnCopy = std::bind(&TabsPanel::CopyTransaction, this, std::placeholders::_1);
	transactionList->OnEdit = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
	transactionList->OnSplit = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);

	tabsPanels[tabIndex] = transactionList;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Account;
	tabsPanels[tabIndex]->id = account->id;

	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, *account->name);
	UpdateTransactionList(transactionList, TreeMenuItemTypes::Account, account);
}

void TabsPanel::CreateAccountsPanel(int tabIndex, TreeMenuItemTypes type) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, wxID_ANY);

	transactionList->OnAdd = std::bind(&TabsPanel::AddTransaction, this);
	transactionList->OnCopy = std::bind(&TabsPanel::CopyTransaction, this, std::placeholders::_1);
	transactionList->OnEdit = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
	transactionList->OnSplit = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);

	tabsPanels[tabIndex] = transactionList;
	tabsPanels[tabIndex]->type = type;

	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	wxString name = wxT("");

	if (type == TreeMenuItemTypes::Deposits) {
		name = wxT("Deposits");
	}
	else if (type == TreeMenuItemTypes::Expenses) {
		name = wxT("Expenses");
	}
	else if (type == TreeMenuItemTypes::Receipts) {
		name = wxT("Receipts");
	}
	else if (type == TreeMenuItemTypes::Accounts) {
		name = wxT("Transactions");
	}

	notebook->SetPageText(tabIndex, name);
	UpdateTransactionList(transactionList, type, nullptr);
}

void TabsPanel::CreateDashboardPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	DashboardPanel *dashboardPanel = new DashboardPanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = dashboardPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Dashboard;

	sizer->Add(dashboardPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Dashboard"));

	dashboardPanel->Update();
}

void TabsPanel::CreateBudgetsPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	BudgetsPanel *budgetPanel = new BudgetsPanel(panel, wxID_ANY);

	budgetPanel->OnAdd = std::bind(&TabsPanel::AddBudget, this);
	budgetPanel->OnEdit = std::bind(&TabsPanel::EditBudget, this, std::placeholders::_1);

	tabsPanels[tabIndex] = budgetPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Budgets;

	sizer->Add(budgetPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Budgets"));

	budgetPanel->Update();
}

void TabsPanel::CreateSchedulersPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	SchedulersPanel *schedulersPanel = new SchedulersPanel(panel, wxID_ANY);

	schedulersPanel->OnAdd = std::bind(&TabsPanel::AddScheduler, this);
	schedulersPanel->OnEdit = std::bind(&TabsPanel::EditScheduler, this, std::placeholders::_1);

	tabsPanels[tabIndex] = schedulersPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Schedulers;

	sizer->Add(schedulersPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Schedulers"));

	schedulersPanel->Update();
}

void TabsPanel::CreateGoalsPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	GoalsPanel *goalsPanel = new GoalsPanel(panel, wxID_ANY);

	goalsPanel->OnAdd = std::bind(&TabsPanel::AddGoal, this);
	goalsPanel->OnEdit = std::bind(&TabsPanel::EditGoal, this, std::placeholders::_1);

	tabsPanels[tabIndex] = goalsPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Goals;

	sizer->Add(goalsPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Goals"));

	goalsPanel->Update();
}

void TabsPanel::CreateReportPanel(int tabIndex, std::shared_ptr<Report> report) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	DataPanel *reportPanel;

	if (report->id == 1) {
		reportPanel = new ReportExpensesByMonthPanel(panel, wxID_ANY);
	}
	else if (report->id == 2) {
		reportPanel = new ReportBalancePanel(panel, wxID_ANY);
	}
	else {
		reportPanel = new ReportExpensesForPeriodPanel(panel, wxID_ANY);
	}

	tabsPanels[tabIndex] = reportPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Report;
	tabsPanels[tabIndex]->id = report->id;

	sizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, *report->name);

	reportPanel->Update();
}

void TabsPanel::CreateTrashPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TrashPanel *trashPanel = new TrashPanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = trashPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Trash;

	sizer->Add(trashPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Trash"));

	trashPanel->Update();
}

void TabsPanel::CreateTagsPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TagsPanel *tagsPanel = new TagsPanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = tagsPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Tags;

	sizer->Add(tagsPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Tags"));

	tagsPanel->Update();
}

void TabsPanel::CreateAlertsPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	AlertsPanel *alertsPanel = new AlertsPanel(panel, wxID_ANY);

	alertsPanel->OnAdd = std::bind(&TabsPanel::AddAlert, this);
	alertsPanel->OnEdit = std::bind(&TabsPanel::EditAlert, this, std::placeholders::_1);

	tabsPanels[tabIndex] = alertsPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::Alerts;

	sizer->Add(alertsPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Alerts"));

	alertsPanel->Update();
}


void TabsPanel::OnTabChanged(wxBookCtrlEvent &event) {
	//
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	contextMenuTab = notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	wxMenu *menu = new wxMenu();

	wxMenuItem *leftItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::MoveLeft), wxT("Move to Left"));
	wxMenuItem *rightItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::MoveRight), wxT("Move to Right"));
	wxMenuItem *closeItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Close), wxT("Close"));

	if (notebook->GetPageCount() == 1) {
		leftItem->Enable(false);
		leftItem->SetTextColour(*wxLIGHT_GREY);

		rightItem->Enable(false);
		rightItem->SetTextColour(*wxLIGHT_GREY);

		closeItem->Enable(false);
		closeItem->SetTextColour(*wxLIGHT_GREY);
	}

	if (contextMenuTab == 0) {
		leftItem->Enable(false);
		leftItem->SetTextColour(*wxLIGHT_GREY);
	}

	if (contextMenuTab == notebook->GetPageCount() - 1) {
		rightItem->Enable(false);
		rightItem->SetTextColour(*wxLIGHT_GREY);
	}

	menu->Append(leftItem);
	menu->Append(rightItem);
	menu->AppendSeparator();
	menu->Append(closeItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsPanel::OnTabMenuClose, this, static_cast<int>(ContextMenuTypes::Close));

	notebook->PopupMenu(menu, point);

	delete menu;

	event.Skip();
}

void TabsPanel::OnTabMenuClose(wxCommandEvent &event) {	
	RemoveTab(contextMenuTab);
}

void TabsPanel::Update() {
	for (unsigned int i = 0; i < tabsPanels.size(); i++) {
		tabsPanels[i]->Update();
	}
}

void TabsPanel::AddTransaction() {
	if (OnAddTransaction) {
		OnAddTransaction();
	}
}

void TabsPanel::CopyTransaction(std::shared_ptr<Transaction> transaction) {
	if (OnCopyTransaction) {
		OnCopyTransaction(transaction);
	}
}

void TabsPanel::EditTransaction(std::shared_ptr<Transaction> transaction) {
	if (OnEditTransaction) {
		OnEditTransaction(transaction);
	}
}

void TabsPanel::SplitTransaction(std::shared_ptr<Transaction> transaction) {
	if (OnSplitTransaction) {
		OnSplitTransaction(transaction);
	}
}

void TabsPanel::UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, std::shared_ptr<Account> account)
{
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
}

std::shared_ptr<Transaction> TabsPanel::GetSelectedTransaction() {
	int i = notebook->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];	

	if (currentPanel->type == TreeMenuItemTypes::Account) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		return transactionList->GetTransaction();
	}

	return nullptr;
}

void TabsPanel::RemoveTab(int index) {
	DataPanel *currentPanel = tabsPanels[index];

	notebook->RemovePage(index);

	tabs.erase(tabs.begin() + index);
	tabsSizer.erase(tabsSizer.begin() + index);
	tabsPanels.erase(tabsPanels.begin() + index);
	
	delete currentPanel;
}

void TabsPanel::AddBudget() {
	if (OnAddBudget) {
		OnAddBudget();
	}
}

void TabsPanel::EditBudget(std::shared_ptr<Budget> budget) {
	if (OnEditBudget) {
		OnEditBudget(budget);
	}
}

void TabsPanel::AddScheduler() {
	if (OnAddScheduler) {
		OnAddScheduler();
	}
}

void TabsPanel::EditScheduler(std::shared_ptr<Scheduler> scheduler) {
	if (OnEditScheduler) {
		OnEditScheduler(scheduler);
	}
}

void TabsPanel::AddGoal() {
	if (OnAddGoal) {
		OnAddGoal();
	}
}

void TabsPanel::EditGoal(std::shared_ptr<Goal> goal) {
	if (OnEditGoal) {
		OnEditGoal(goal);
	}
}


void TabsPanel::AddAlert() {
	if (OnAddAlert) {
		OnAddAlert();
	}
}

void TabsPanel::EditAlert(std::shared_ptr<Alert> alert) {
	if (OnEditAlert) {
		OnEditAlert(alert);
	}
}