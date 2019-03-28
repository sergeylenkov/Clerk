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

	for each (auto panel in tabsPanels)
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
	for each (auto tab in Settings::GetInstance().GetTabs()) {
		TreeMenuItemTypes type = (TreeMenuItemTypes)tab.type;

		if (type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> tabAccount = DataHelper::GetInstance().GetAccountById(tab.id);

			if (tabAccount) {
				CreateTab(type, tabAccount);
			}
		} else if (type == TreeMenuItemTypes::MenuReport) {
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

	for each (auto tabPanel in tabsPanels) {
		if (type == TreeMenuItemTypes::MenuAccount) {
			if (tabPanel->type == type && tabPanel->id == id) {
				return true;
			}
		}
		else if (type == TreeMenuItemTypes::MenuReports) {
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

		if (type == TreeMenuItemTypes::MenuAccount) {
			if (tabPanel->type == type && tabPanel->id == id) {
				notebook->ChangeSelection(i);
			}
		}
		else if (type == TreeMenuItemTypes::MenuReports) {
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
	if (type == TreeMenuItemTypes::MenuAccount) {
		auto account = std::static_pointer_cast<Account>(object);
		CreateAccountPanel(tabIndex, account);
	}
	else if (type == TreeMenuItemTypes::MenuAccounts || type == TreeMenuItemTypes::MenuReceipts 
		|| type == TreeMenuItemTypes::MenuExpenses || type == TreeMenuItemTypes::MenuDeposits || type == TreeMenuItemTypes::MenuVirtual) {
		CreateAccountsPanel(tabIndex, (TreeMenuItemTypes)type);
	}
	else if (type == TreeMenuItemTypes::MenuDashboard) {
		CreateDashboardPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuBudgets) {
		CreateBudgetsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuSchedulers) {
		CreateSchedulersPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuGoals) {
		CreateGoalsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuTrash) {
		CreateTrashPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuTags) {
		CreateTagsPanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuReport) {
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
	transactionList->OnPeriodChanged = std::bind(&TabsPanel::UpdateStatus, this);

	tabsPanels[tabIndex] = transactionList;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuAccount;
	tabsPanels[tabIndex]->id = account->id;

	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, *account->name);
	UpdateTransactionList(transactionList, TreeMenuItemTypes::MenuAccount, account);
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
	transactionList->OnPeriodChanged = std::bind(&TabsPanel::UpdateStatus, this);

	tabsPanels[tabIndex] = transactionList;
	tabsPanels[tabIndex]->type = type;

	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	wxString name = wxT("");

	if (type == TreeMenuItemTypes::MenuDeposits) {
		name = wxT("Deposits");
	}
	else if (type == TreeMenuItemTypes::MenuExpenses) {
		name = wxT("Expenses");
	}
	else if (type == TreeMenuItemTypes::MenuReceipts) {
		name = wxT("Receipts");
	}
	else if (type == TreeMenuItemTypes::MenuAccounts) {
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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuDashboard;

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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuBudgets;

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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuSchedulers;

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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuGoals;

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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuReport;
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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuTrash;

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
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuTags;

	sizer->Add(tagsPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	notebook->SetPageText(tabIndex, wxT("Tags"));

	tagsPanel->Update();
}

void TabsPanel::OnTabChanged(wxBookCtrlEvent &event) {
	//int i = notebook->GetSelection();
	//tabsPanels[i]->Update();
	//UpdateStatus();
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	contextMenuTab = notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	wxMenu *menu = new wxMenu();

	wxMenuItem *leftItem = new wxMenuItem(menu, static_cast<int>(TabsMenuTypes::MoveLeft), wxT("Move to Left"));
	wxMenuItem *rightItem = new wxMenuItem(menu, static_cast<int>(TabsMenuTypes::MoveRight), wxT("Move to Right"));
	wxMenuItem *closeItem = new wxMenuItem(menu, static_cast<int>(TabsMenuTypes::Close), wxT("Close"));

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

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsPanel::OnTabMenuClose, this, static_cast<int>(TabsMenuTypes::Close));

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

void TabsPanel::UpdateStatus() {
	int i = notebook->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];
	wxString result = wxT("");

	if (currentPanel->type == TreeMenuItemTypes::MenuAccount) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		auto account = transactionList->GetAccount();
		float amount = 0;

		if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Credit) {
			amount = DataHelper::GetInstance().GetBalance(account.get());
		}
		else if (account->type == AccountTypes::Expens) {
			amount = transactionList->GetBalance();
		}
		else {
			amount = DataHelper::GetInstance().GetToAmountSum(account.get(), &transactionList->GetFromDate(), &transactionList->GetToDate());
		}

		wxString name = *account->name;

		if (account->creditLimit > 0.0) {
			result = wxString::Format("%s: %.2f (%.2f %.2f) %s", static_cast<const char*>(name.c_str()), account->creditLimit + amount, account->creditLimit, amount, static_cast<const char*>(account->currency->shortName->c_str()));
		}
		else {
			result = wxString::Format("%s: %.2f %s", static_cast<const char*>(name.c_str()), amount, static_cast<const char*>(account->currency->shortName->c_str()));
		}
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuExpenses) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		result = wxString::Format("Expenses: %.2f", amount);
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuReceipts) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		result = wxString::Format("Receipts: %.2f", amount);
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuDeposits) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		result = wxString::Format("Deposits: %.2f", amount);
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuDashboard) {
		wxDateTime fromDate = wxDateTime::Now();
		wxDateTime toDate = wxDateTime::Now();

		fromDate.SetDay(1);
		toDate.SetToLastMonthDay();

		float expenses = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
		float receipts = DataHelper::GetInstance().GetReceipts(&fromDate, &toDate);

		result = wxString::Format("%s: Expenses: %.2f, Receipts: %.2f", wxDateTime::Now().Format("%B"), expenses, receipts);
	}

	if (OnUpdateStatus) {
		OnUpdateStatus(result);
	}
}

void TabsPanel::UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, std::shared_ptr<Account> account)
{
	if (type == TreeMenuItemTypes::MenuAccount) {
		transactionList->SetType(type);
		transactionList->SetAccount(account);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::MenuExpenses) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::MenuReceipts) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::MenuDeposits) {
		transactionList->SetType(type);
		transactionList->Update();
	}
	else if (type == TreeMenuItemTypes::MenuAccounts) {
		transactionList->SetType(type);
		transactionList->Update();
	}

	UpdateStatus();
}

std::shared_ptr<Transaction> TabsPanel::GetSelectedTransaction() {
	int i = notebook->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];	

	if (currentPanel->type == TreeMenuItemTypes::MenuAccount) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		return transactionList->GetTransaction();
	}

	return nullptr;
}

std::shared_ptr<Budget> TabsPanel::GetSelectedBudget() {
	int i = notebook->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];

	if (currentPanel->type == TreeMenuItemTypes::MenuBudgets) {
		BudgetsPanel *budgetsList = (BudgetsPanel *)currentPanel;
		return budgetsList->GetBudget();
	}

	return nullptr;
}

std::shared_ptr<Account> TabsPanel::GetSelectedAccount() {
	int i = notebook->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];

	if (currentPanel->type == TreeMenuItemTypes::MenuAccount) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		return transactionList->GetAccount();
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