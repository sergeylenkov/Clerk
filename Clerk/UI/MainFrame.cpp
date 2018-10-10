#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	Settings::GetInstance().Open("Config.txt");

	this->SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	splitter->SetSashGravity(0.5);
	splitter->SetMinimumPaneSize(300);

	wxPanel *splitterLeftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	splitterLeftPanel->SetBackgroundColour(wxColour(245, 245, 245, 1));

	wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new TreeMenu(splitterLeftPanel, wxID_ANY);

	treeMenu->OnAccountSelect = std::bind(&MainFrame::OnTreeMenuAccountSelect, this, std::placeholders::_1);
	treeMenu->OnReportSelect = std::bind(&MainFrame::OnTreeMenuReportSelect, this, std::placeholders::_1);
	treeMenu->OnDashboardSelect = std::bind(&MainFrame::OnTreeMenuDashboardSelect, this);
	treeMenu->OnBudgetsSelect = std::bind(&MainFrame::OnTreeMenuBudgetsSelect, this);
	treeMenu->OnSchedulersSelect = std::bind(&MainFrame::OnTreeMenuSchedulersSelect, this);
	treeMenu->OnTrashSelect = std::bind(&MainFrame::OnTreeMenuTrashSelect, this);
	treeMenu->OnAccountsSelect = std::bind(&MainFrame::OnTreeMenuAccountsSelect, this, std::placeholders::_1);
	treeMenu->OnAddAccount = std::bind(&MainFrame::AddAccount, this);
	treeMenu->OnEditAccount = std::bind(&MainFrame::EditAccount, this, std::placeholders::_1);
	treeMenu->OnAddTransaction = std::bind(&MainFrame::OnTreeMenuAddTransaction, this, std::placeholders::_1);
	treeMenu->OnNewTab = std::bind(&MainFrame::AddTab, this, std::placeholders::_1, std::placeholders::_2);

	boxSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);

	CreateMainMenu();
	CreateStatusBar();
	SetStatusText("");

	wxPanel *splitterRightPanel = new wxPanel(splitter, wxID_ANY);	

	rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	tabsPanel = new TabsPanel(splitterRightPanel, wxID_ANY);

	tabsPanel->OnUpdateStatus = std::bind(&MainFrame::UpdateStatus, this, std::placeholders::_1);
	tabsPanel->OnAddTransaction = std::bind(&MainFrame::AddTransactionFromContextMenu, this);
	tabsPanel->OnCopyTransaction = std::bind(&MainFrame::CopyTransaction, this, std::placeholders::_1);
	tabsPanel->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this, std::placeholders::_1);
	tabsPanel->OnSplitTransaction = std::bind(&MainFrame::SplitTransaction, this, std::placeholders::_1);
	tabsPanel->OnAddBudget = std::bind(&MainFrame::AddBudget, this);
	tabsPanel->OnEditBudget = std::bind(&MainFrame::EditBudget, this, std::placeholders::_1);
	tabsPanel->OnAddScheduler = std::bind(&MainFrame::AddScheduler, this);
	tabsPanel->OnEditScheduler = std::bind(&MainFrame::EditScheduler, this, std::placeholders::_1);

	rightPanelSizer->Add(tabsPanel, 1, wxEXPAND | wxALL, 0);
	rightPanelSizer->Layout();

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);

	treeMenu->Update();
	treeMenu->RestoreState();

	tabsPanel->RestoreTabs();
	tabsPanel->UpdateStatus();

	CheckSchedulers();
}

MainFrame::~MainFrame() 
{
	delete treeMenu;
	delete tabsPanel;

	Settings::GetInstance().SetWindowWidth(this->GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(this->GetSize().GetHeight());

	Settings::GetInstance().Save();
}

void MainFrame::CreateMainMenu() {
	wxMenu *menuFile = new wxMenu();
	wxMenu *menuHelp = new wxMenu();

	menuHelp->Append(wxID_ABOUT, "&About...");
	
	menuFile->Append(ID_ADD_TRANSACTION, wxT("Add Transaction...\tCtrl+T"));
	menuFile->AppendSeparator();
	menuFile->Append(ID_ADD_ACCOUNT, wxT("Add Account..."));
	menuFile->Append(ID_ADD_BUDGET, wxT("Add Budget..."));
	menuFile->Append(ID_ADD_SCHEDULER, wxT("Add Scheduler..."));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "E&xit\tCtrl+W");

	//wxMenu *menuTransactions = new wxMenu();

	//menuTransactions->Append(ID_DUPLICATE_TRANSACTION, wxT("Duplicate...\tCtrl+D"));
	//menuTransactions->Append(ID_SPLIT_TRANSACTION, wxT("Split...\tCtrl+S"));	

	wxMenuBar *menuBar = new wxMenuBar();

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");	

	SetMenuBar(menuBar);

	menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);

	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransaction, this, ID_ADD_TRANSACTION);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, ID_ADD_ACCOUNT);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddBudget, this, ID_ADD_BUDGET);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddScheduler, this, ID_ADD_SCHEDULER);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);	

	//menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransaction, this, ID_ADD_TRANSACTION);
	//menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnDuplicateTransaction, this, ID_DUPLICATE_TRANSACTION);
	//menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSplitTransaction, this, ID_SPLIT_TRANSACTION);	
}

void MainFrame::UpdateStatus(wxString text) {
	SetStatusText(text);	
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(wxString::Format("Version: %s", "0.1"), "About Clerk", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnAddAccount(wxCommandEvent &event) {
	AddAccount();
}

void MainFrame::OnTreeMenuAccountSelect(std::shared_ptr<Account> account) {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuAccount, account->id)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuAccount, account->id);
	}
	else {		
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuAccount, account);
	}
}

void MainFrame::OnTreeMenuReportSelect(std::shared_ptr<Report> report) {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuReport, report->id)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuReport, report->id);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuReport, report);
	}
}

void MainFrame::OnTreeMenuDashboardSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuDashboard)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuDashboard);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuDashboard, nullptr);
	}
}

void MainFrame::OnTreeMenuBudgetsSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuBudgets)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuBudgets);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuBudgets, nullptr);
	}
}

void MainFrame::OnTreeMenuSchedulersSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuSchedulers)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuSchedulers);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuSchedulers, nullptr);
	}
}

void MainFrame::OnTreeMenuTrashSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::MenuTrash)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::MenuTrash);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::MenuTrash, nullptr);
	}
}


void MainFrame::OnTreeMenuAccountsSelect(TreeMenuItemTypes type) {
	if (tabsPanel->IsTabExists(type)) {
		tabsPanel->SelectTab(type);
	}
	else {
		tabsPanel->AddTab(type, nullptr);
	}
}

void MainFrame::OnTreeMenuAddTransaction(std::shared_ptr<Account> account) {
	AddTransaction(account.get());
}

void MainFrame::OnAddTransaction(wxCommandEvent &event) {
	auto account = tabsPanel->GetSelectedAccount();
	AddTransaction(account.get());
}

void MainFrame::OnDuplicateTransaction(wxCommandEvent &event) {
	auto transaction = tabsPanel->GetSelectedTransaction();

	if (transaction) {
		Transaction *copy = new Transaction();

		copy->fromAccountId = transaction->fromAccountId;
		copy->toAccountId = transaction->toAccountId;
		copy->fromAmount = transaction->fromAmount;
		copy->toAmount = transaction->toAmount;
		copy->note = transaction->note;
		copy->tags = transaction->tags;
		copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

		copy->Save();

		delete copy;

		tabsPanel->Update();
	}	
}

void MainFrame::OnSplitTransaction(wxCommandEvent &event) {
	auto transaction = tabsPanel->GetSelectedTransaction();

	if (transaction) {
		SplitTransaction(transaction);
	}
}

void MainFrame::AddTransaction(Account *account) {
	auto transaction = make_shared<Transaction>();

	if (account) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			transaction->fromAccountId = account->id;
			transaction->toAccountId = DataHelper::GetInstance().GetPairAccountId(account);
		}

		if (account->type == AccountTypes::Expens || account->type == AccountTypes::Credit) {
			transaction->toAccountId = account->id;
			transaction->fromAccountId = DataHelper::GetInstance().GetPairAccountId(account);
		}
	}

	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);

	transactionFrame->SetTransaction(transaction);
	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);

	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();
}

void MainFrame::AddTransactionFromContextMenu() {
	auto transaction = tabsPanel->GetSelectedTransaction();

	if (transaction) {
		auto copy = make_shared<Transaction>();

		copy->fromAccountId = transaction->fromAccountId;
		copy->toAccountId = transaction->toAccountId;		

		EditTransaction(copy);
	}
	else {
		AddTransaction(nullptr);
	}
}

void MainFrame::CopyTransaction(std::shared_ptr<Transaction> transaction) {
	auto copy = make_shared<Transaction>();

	copy->fromAccountId = transaction->fromAccountId;
	copy->toAccountId = transaction->toAccountId;
	copy->fromAmount = transaction->fromAmount;
	copy->toAmount = transaction->toAmount;
	copy->note = transaction->note;
	copy->tags = transaction->tags;
	copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);

	transactionFrame->SetTransaction(copy);
	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);

	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();
}

void MainFrame::EditTransaction(std::shared_ptr<Transaction> transaction) {
	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);
	
	transactionFrame->SetTransaction(transaction);
	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);

	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();
}

void MainFrame::SplitTransaction(std::shared_ptr<Transaction> transaction) {
	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);
	
	transactionFrame->SetSplitTransaction(transaction);
	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);

	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();
}

void MainFrame::OnTransactionClose() {
	delete transactionFrame;

	tabsPanel->Update();
}

void MainFrame::AddAccount() {
	std::shared_ptr<Account> account = make_shared<Account>();
	
	accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 340, 400);

	accountFrame->SetAccount(account);
	accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);

	accountFrame->Show(true);
	accountFrame->CenterOnParent();
}

void MainFrame::EditAccount(std::shared_ptr<Account> account) {
	accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 340, 400);
		
	accountFrame->SetAccount(account);
	accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);

	accountFrame->Show(true);
	accountFrame->CenterOnParent();
}

void MainFrame::DeleteAccount(std::shared_ptr<Account> account) {
	account->Delete();
	treeMenu->Update();	
}

void MainFrame::OnAccountClose() {
	delete accountFrame;
	treeMenu->Update();
}

void MainFrame::OnAddBudget(wxCommandEvent &event) {
	AddBudget();
}

void MainFrame::AddBudget() {
	std::shared_ptr<Budget> budget = make_shared<Budget>();
	
	budgetFrame = new BudgetFrame(this, wxT("Budget"), 0, 0, 340, 400);

	budgetFrame->SetBudget(budget);
	budgetFrame->OnClose = std::bind(&MainFrame::OnBudgetClose, this);

	budgetFrame->Show(true);
	budgetFrame->CenterOnParent();
}

void MainFrame::EditBudget(std::shared_ptr<Budget> budget) {
	budgetFrame = new BudgetFrame(this, wxT("Budget"), 0, 0, 340, 400);

	budgetFrame->SetBudget(budget);
	budgetFrame->OnClose = std::bind(&MainFrame::OnBudgetClose, this);

	budgetFrame->Show(true);
	budgetFrame->CenterOnParent();
}

void MainFrame::OnBudgetClose() {
	delete budgetFrame;

	tabsPanel->Update();
}

void MainFrame::OnAddScheduler(wxCommandEvent &event) {
	AddScheduler();
}

void MainFrame::AddScheduler() {
	std::shared_ptr<Scheduler> scheduler = make_shared<Scheduler>();

	schedulerFrame = new SchedulerFrame(this, wxT("Scheduler"), 0, 0, 450, 480);
	
	schedulerFrame->SetScheduler(scheduler);
	schedulerFrame->OnClose = std::bind(&MainFrame::OnSchedulerClose, this);

	schedulerFrame->Show(true);
	schedulerFrame->CenterOnParent();
}

void MainFrame::EditScheduler(std::shared_ptr<Scheduler> scheduler) {
	schedulerFrame = new SchedulerFrame(this, wxT("Scheduler"), 0, 0, 450, 480);

	schedulerFrame->SetScheduler(scheduler);
	schedulerFrame->OnClose = std::bind(&MainFrame::OnSchedulerClose, this);

	schedulerFrame->Show(true);
	schedulerFrame->CenterOnParent();
}

void MainFrame::OnSchedulerClose() {
	tabsPanel->Update();
}

void MainFrame::AddTab(TreeMenuItemTypes type, shared_ptr<void> object) {
	tabsPanel->AddTab(type, object);
}

void MainFrame::CheckSchedulers() {
	wxDateTime today = wxDateTime::Now();
	today.ResetTime();

	std::vector<shared_ptr<Scheduler>> schedulers;

	for each (auto scheduler in DataHelper::GetInstance().GetSchedulers())
	{
		if (scheduler->active && today.IsLaterThan(*scheduler->nextDate)) {			
			schedulers.push_back(scheduler);
		}
	}

	if (schedulers.size() > 0) {
		SchedulersConfirmFrame *confirmFrame = new SchedulersConfirmFrame(this, wxT("Schedulers"), 0, 0, 450, 400);

		confirmFrame->SetSchedulers(schedulers);

		confirmFrame->Show(true);
		confirmFrame->CenterOnParent();
	}
}