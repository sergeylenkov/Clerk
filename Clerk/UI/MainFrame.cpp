#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	Settings::GetInstance().Open("Config.txt");

	this->SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));
	
	wxMenu *menuFile = new wxMenu();

	menuFile->Append(wxID_ABOUT, "&About...");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "E&xit\tCtrl+W");

	wxMenu *menuAccounts = new wxMenu();
	menuAccounts->Append(ID_ADD_ACCOUNT, wxT("Add Account...\tCtrl+N"));

	wxMenu *menuTransactions = new wxMenu();
	menuTransactions->Append(ID_ADD_TRANSACTION, wxT("Add...\tCtrl+T"));
	menuTransactions->Append(ID_DUPLICATE_TRANSACTION, wxT("Duplicate...\tCtrl+D"));
	menuTransactions->Append(ID_SPLIT_TRANSACTION, wxT("Split...\tCtrl+S"));

	wxMenu *menuBudgets = new wxMenu();
	menuBudgets->Append(ID_ADD_BUDGET, wxT("Add Budget..."));

	wxMenuBar *menuBar = new wxMenuBar();

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuAccounts, "&Accounts");
	menuBar->Append(menuTransactions, "&Transactions");
	menuBar->Append(menuBudgets, "&Budgets");

	SetMenuBar(menuBar);

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	splitter->SetSashGravity(0.5);
	splitter->SetMinimumPaneSize(300);

	wxPanel *splitterLeftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	splitterLeftPanel->SetBackgroundColour(wxColour(245, 245, 245, 1));

	wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new TreeMenu(splitterLeftPanel, wxID_ANY);

	treeMenu->OnAccountSelect = std::bind(&MainFrame::OnTreeMenuAccountSelect, this, std::placeholders::_1);
	treeMenu->OnReportSelect = std::bind(&MainFrame::OnTreeMenuReportSelect, this, std::placeholders::_1);
	treeMenu->OnHomeSelect = std::bind(&MainFrame::OnTreeMenuHomeSelect, this);
	treeMenu->OnBudgetsSelect = std::bind(&MainFrame::OnTreeMenuBudgetsSelect, this);
	treeMenu->OnAccountsSelect = std::bind(&MainFrame::OnTreeMenuAccountsSelect, this, std::placeholders::_1);
	treeMenu->OnAddAccount = std::bind(&MainFrame::AddAccount, this);
	treeMenu->OnEditAccount = std::bind(&MainFrame::EditAccount, this, std::placeholders::_1);
	treeMenu->OnAddTransaction = std::bind(&MainFrame::AddTransaction, this, std::placeholders::_1);
	treeMenu->OnNewTab = std::bind(&MainFrame::AddTab, this, std::placeholders::_1, std::placeholders::_2);

	boxSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);

	CreateStatusBar();
	SetStatusText("");

	wxPanel *splitterRightPanel = new wxPanel(splitter, wxID_ANY);	

	rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	tabsPanel = new wxNotebook(splitterRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);	

	rightPanelSizer->Add(tabsPanel, 1, wxEXPAND | wxALL, 0);
	tabsPanel->Show();
	rightPanelSizer->Layout();

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);

	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);

	menuAccounts->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, ID_ADD_ACCOUNT);

	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransaction, this, ID_ADD_TRANSACTION);
	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnDuplicateTransaction, this, ID_DUPLICATE_TRANSACTION);
	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSplitTransaction, this, ID_SPLIT_TRANSACTION);

	menuBudgets->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddBudget, this, ID_ADD_BUDGET);

	tabsPanel->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnTabChanged, this);

	treeMenu->Update();
	treeMenu->RestoreState();

	RestoreTabs();
	UpdateStatus();
}

MainFrame::~MainFrame() 
{
	Settings::GetInstance().SetWindowWidth(this->GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(this->GetSize().GetHeight());

	Settings::GetInstance().ClearTabs();

	for each (auto panel in tabsPanels)
	{
		Settings::GetInstance().AddTab(panel->type, panel->id);			
	}

	Settings::GetInstance().SetSelectedTab(tabsPanel->GetSelection());

	Settings::GetInstance().Save();
	
	delete treeMenu;
}

void MainFrame::UpdateTransactionList(TransactionsListPanel *transactionList, TreeMenuItemTypes type, Account *account)
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

	UpdateStatus();
}

void MainFrame::UpdateStatus() {
	int i = tabsPanel->GetSelection();
	DataPanel *currentPanel = tabsPanels[i];

	if (currentPanel->type == TreeMenuItemTypes::MenuAccount) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;
		Account *account = transactionList->GetAccount();
		float amount = 0;

		if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Credit) {
			amount = DataHelper::GetInstance().GetBalance(account);
		}
		else if (account->type == AccountTypes::Expens) {
			amount = transactionList->GetBalance();
		}
		else {
			amount = DataHelper::GetInstance().GetToAmountSum(account, &transactionList->GetFromDate(), &transactionList->GetToDate());
		}

		wxString name = *account->name;

		if (account->creditLimit > 0.0) {
			SetStatusText(wxString::Format("%s: %.2f (%.2f %.2f) %s", static_cast<const char*>(name), account->creditLimit + amount, account->creditLimit, amount, static_cast<const char*>(account->currency->shortName->c_str())));
		}
		else {
			SetStatusText(wxString::Format("%s: %.2f %s", static_cast<const char*>(name), amount, static_cast<const char*>(account->currency->shortName->c_str())));
		}
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuExpenses) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Expenses: %.2f", amount));
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuReceipts) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Receipts: %.2f", amount));
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuDeposits) {
		TransactionsListPanel *transactionList = (TransactionsListPanel *)currentPanel;

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Deposits: %.2f", amount));
	}
	else if (currentPanel->type == TreeMenuItemTypes::MenuHome) {
		wxDateTime fromDate = wxDateTime::Now();
		wxDateTime toDate = wxDateTime::Now();

		fromDate.SetDay(1);
		toDate.SetToLastMonthDay();

		float expenses = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
		float receipts = DataHelper::GetInstance().GetReceipts(&fromDate, &toDate);

		SetStatusText(wxString::Format("%s: Expenses: %.2f, Receipts: %.2f", wxDateTime::Now().Format("%B"), expenses, receipts));
	}
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox("Personal finance application", "About Clerk", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnAddAccount(wxCommandEvent &event) {
	AddAccount();
}

void MainFrame::OnTreeMenuAccountSelect(std::shared_ptr<Account> account) {
	if (IsTabExists(TreeMenuItemTypes::MenuAccount, account->id)) {
		SelectTab(TreeMenuItemTypes::MenuAccount, account->id);
	}
	else {		
		int i = tabsPanel->GetSelection();
		CreateAccountPanel(i, account);
	}
}

void MainFrame::OnTreeMenuReportSelect(std::shared_ptr<Report> report) {
	if (IsTabExists(TreeMenuItemTypes::MenuReport, report->id)) {
		SelectTab(TreeMenuItemTypes::MenuReport, report->id);
	}
	else {
		int i = tabsPanel->GetSelection();
		CreateReportPanel(i, report);
	}
}

void MainFrame::OnTreeMenuHomeSelect() {
	if (IsTabExists(TreeMenuItemTypes::MenuHome)) {
		SelectTab(TreeMenuItemTypes::MenuHome);
	}
	else {
		int i = tabsPanel->GetSelection();
		CreateHomePanel(i);
	}
}

void MainFrame::OnTreeMenuBudgetsSelect() {
	if (IsTabExists(TreeMenuItemTypes::MenuBudgets)) {
		SelectTab(TreeMenuItemTypes::MenuBudgets);
	}
	else {
		int i = tabsPanel->GetSelection();
		CreateBudgetsPanel(i);
	}
}

void MainFrame::OnTreeMenuAccountsSelect(TreeMenuItemTypes type) {
	if (IsTabExists(type)) {
		SelectTab(type);
	}
	else {
		int i = tabsPanel->GetSelection();
		CreateAccountsPanel(i, type);
	}
}

void MainFrame::OnAddTransaction(wxCommandEvent &event) {
	int i = tabsPanel->GetSelection();
	std::shared_ptr<Account> account;

	if (tabsPanels[i]->type == TreeMenuItemTypes::MenuAccount) {
		account = DataHelper::GetInstance().GetAccountById(tabsPanels[i]->id);
	}

	AddTransaction(account);
}

void MainFrame::OnDuplicateTransaction(wxCommandEvent &event) {
	TransactionsListPanel *list = (TransactionsListPanel *)tabsPanels[tabsPanel->GetSelection()];
	list->DublicateTransaction();
}

void MainFrame::OnSplitTransaction(wxCommandEvent &event) {
	SplitTransaction();
}

void MainFrame::OnOpenNewTab(wxCommandEvent &event) {

}

void MainFrame::AddTransaction(std::shared_ptr<Account> account) {
	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);
	
	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();

	auto transaction = make_shared<Transaction>();

	transactionFrame->SetTransaction(transaction);

	if (account) {
		transactionFrame->SetAccount(account);
	}	

	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
}

void MainFrame::EditTransaction() {
	TransactionsListPanel *list = (TransactionsListPanel *)tabsPanels[tabsPanel->GetSelection()];
	auto transaction = list->GetTransaction();

	if (transaction) {
		transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);

		transactionFrame->Show(true);
		transactionFrame->CenterOnParent();

		transactionFrame->SetTransaction(transaction);
		transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
	}
}

void MainFrame::SplitTransaction() {
	TransactionsListPanel *list = (TransactionsListPanel *)tabsPanels[tabsPanel->GetSelection()];
	auto transaction = list->GetTransaction();

	if (transaction) {
		transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);

		transactionFrame->Show(true);
		transactionFrame->CenterOnParent();

		transactionFrame->SetSplitTransaction(transaction);
		transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
	}
}

void MainFrame::OnTransactionClose() {
	delete transactionFrame;

	for (unsigned int i = 0; i < tabsPanels.size(); i++) {
		tabsPanels[i]->Update();
	}

	UpdateStatus();
}

void MainFrame::AddAccount() {
	accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 340, 400);

	accountFrame->Show(true);
	accountFrame->CenterOnParent();

	std::shared_ptr<Account> account = make_shared<Account>();

	account->id = -1;
	account->name = make_shared<wxString>("");
	account->note = make_shared<wxString>("");
	account->type = AccountTypes::Deposit;
	account->iconId = 0;
	account->orderId = 1000;
	account->currency = make_shared<Currency>(152);
	
	accountFrame->SetAccount(account);

	accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);
}

void MainFrame::EditAccount(std::shared_ptr<Account> account) {
	accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 340, 400);

	accountFrame->Show(true);
	accountFrame->CenterOnParent();

	accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);
	
	accountFrame->SetAccount(account);
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
	budgetFrame = new BudgetFrame(this, wxT("Budget"), 0, 0, 340, 400);

	budgetFrame->Show(true);
	budgetFrame->CenterOnParent();

	std::shared_ptr<Budget> budget = make_shared<Budget>();

	budget->id = -1;
	budget->name = make_shared<wxString>("");
	budget->type = BudgetTypes::Limit;
	budget->period = BudgetPeriods::Month;
	budget->amount = 0.0;

	budgetFrame->SetBudget(budget);

	budgetFrame->OnClose = std::bind(&MainFrame::OnBudgetClose, this);
}

void MainFrame::EditBudget() {
	BudgetsListPanel *list = (BudgetsListPanel *)tabsPanels[tabsPanel->GetSelection()];
	auto budget = list->GetBudget();

	if (budget) {
		budgetFrame = new BudgetFrame(this, wxT("Budget"), 0, 0, 340, 400);

		budgetFrame->Show(true);
		budgetFrame->CenterOnParent();

		budgetFrame->SetBudget(budget);
		budgetFrame->OnClose = std::bind(&MainFrame::OnBudgetClose, this);
	}
}

void MainFrame::OnBudgetClose() {
	delete budgetFrame;

	UpdateBudgetList();
}

void MainFrame::UpdateBudgetList() {
	BudgetsListPanel *list = (BudgetsListPanel *)tabsPanels[tabsPanel->GetSelection()];
	list->Update();
}

void MainFrame::CreateTab(int type, shared_ptr<void> object) {
	wxPanel *panel = new wxPanel(tabsPanel);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(sizer);

	tabs.push_back(panel);
	tabsSizer.push_back(sizer);
	tabsPanels.push_back(nullptr);

	tabsPanel->AddPage(panel, "");

	int index = tabs.size() - 1;

	if (type == TreeMenuItemTypes::MenuAccount) {
		auto account = std::static_pointer_cast<Account>(object);
		CreateAccountPanel(index, account);
	}
	else if (type == TreeMenuItemTypes::MenuHome) {
		CreateHomePanel(index);
	}
	else if (type == TreeMenuItemTypes::MenuBudgets) {
		CreateBudgetsPanel(index);
	}
}

void MainFrame::AddTab(int type, shared_ptr<void> object) {
	CreateTab(type, object);
	tabsPanel->ChangeSelection(tabs.size() - 1);
}

void MainFrame::OnTabChanged(wxBookCtrlEvent &event) {
	int i = tabsPanel->GetSelection();
	tabsPanels[i]->Update();

	UpdateStatus();
}

void MainFrame::RestoreTabs() {
	for each (auto tab in Settings::GetInstance().GetTabs()) {
		if (tab.type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> tabAccount;

			for each (auto account in treeMenu->GetAccounts()) {
				if (account->id == tab.id) {
					tabAccount = account;
					break;
				}
			}

			if (tabAccount) {
				CreateTab(tab.type, tabAccount);
			}
		}
		else {
			CreateTab(tab.type, nullptr);
		}
	}

	int index = Settings::GetInstance().GetSelectedTab();
	tabsPanel->ChangeSelection(index);
}

bool MainFrame::IsTabExists(int type, int id) {
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

void MainFrame::SelectTab(int type, int id) {
	for (unsigned int i = 0; i < tabsPanels.size(); i++) {
		DataPanel *tabPanel = tabsPanels[i];

		if (type == TreeMenuItemTypes::MenuAccount) {
			if (tabPanel->type == type && tabPanel->id == id) {
				tabsPanel->ChangeSelection(i);
			}
		}
		else if (type == TreeMenuItemTypes::MenuReports) {
			if (tabPanel->type == type && tabPanel->id == id) {
				tabsPanel->ChangeSelection(i);
			}
		}
		else if (tabPanel->type == type) {
			tabsPanel->ChangeSelection(i);
		}
	}
}

void MainFrame::CreateAccountPanel(int tabIndex, std::shared_ptr<Account> account) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, wxID_ANY);
	transactionList->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this);
	transactionList->OnSplitTransaction = std::bind(&MainFrame::SplitTransaction, this);
	transactionList->OnPeriodChanged = std::bind(&MainFrame::UpdateStatus, this);

	tabsPanels[tabIndex] = transactionList;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuAccount;
	tabsPanels[tabIndex]->id = account->id;

	sizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	tabsPanel->SetPageText(tabIndex, *account->name);
	UpdateTransactionList(transactionList, TreeMenuItemTypes::MenuAccount, account.get());
}

void MainFrame::CreateAccountsPanel(int tabIndex, TreeMenuItemTypes type) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	TransactionsListPanel *transactionList = new TransactionsListPanel(panel, wxID_ANY);
	transactionList->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this);
	transactionList->OnSplitTransaction = std::bind(&MainFrame::SplitTransaction, this);
	transactionList->OnPeriodChanged = std::bind(&MainFrame::UpdateStatus, this);

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

	tabsPanel->SetPageText(tabIndex, name);
	UpdateTransactionList(transactionList, type, nullptr);
}

void MainFrame::CreateHomePanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	HomePanel *homePanel = new HomePanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = homePanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuHome;

	sizer->Add(homePanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	tabsPanel->SetPageText(tabIndex, wxT("Home"));

	homePanel->Update();
}

void MainFrame::CreateBudgetsPanel(int tabIndex) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	BudgetsListPanel *budgetPanel = new BudgetsListPanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = budgetPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuBudgets;

	sizer->Add(budgetPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	tabsPanel->SetPageText(tabIndex, wxT("Budgets"));

	budgetPanel->Update();
}

void MainFrame::CreateReportPanel(int tabIndex, std::shared_ptr<Report> report) {
	wxPanel *panel = tabs[tabIndex];
	wxBoxSizer *sizer = tabsSizer[tabIndex];
	DataPanel *currentPanel = tabsPanels[tabIndex];

	if (currentPanel) {
		currentPanel->Destroy();
	}

	ReportPanel *reportPanel = new ReportPanel(panel, wxID_ANY);

	tabsPanels[tabIndex] = reportPanel;
	tabsPanels[tabIndex]->type = TreeMenuItemTypes::MenuReport;
	tabsPanels[tabIndex]->id = report->id;

	sizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);
	sizer->Layout();

	tabsPanel->SetPageText(tabIndex, *report->name);

	reportPanel->Update();
}