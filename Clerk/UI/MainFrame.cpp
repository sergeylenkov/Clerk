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
	treeMenu->OnAddAccount = std::bind(&MainFrame::AddAccount, this);
	treeMenu->OnEditAccount = std::bind(&MainFrame::EditAccount, this, std::placeholders::_1);
	treeMenu->OnAddTransaction = std::bind(&MainFrame::AddTransaction, this);
		
	boxSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);

	CreateStatusBar();
	SetStatusText("");

	wxPanel *splitterRightPanel = new wxPanel(splitter, wxID_ANY);	

	rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	//tabsPanel = new wxNotebook(splitterRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	//tabsPanel->AddPage(new wxNotebookPage(tabsPanel, -1), wxT("TEST RECOMMENDATIONS 1"));
	//vbox->Add(tabsPanel, 1, wxEXPAND | wxALL, 0);
	//vbox->Layout();

	transactionsPanelPlaceholder = new wxPanel(splitterRightPanel, wxID_ANY);
	homePanelPlaceholder = new wxPanel(splitterRightPanel, wxID_ANY);
	reportPanelPlaceholder = new wxPanel(splitterRightPanel, wxID_ANY);
	budgetsPanelPlaceholder = new wxPanel(splitterRightPanel, wxID_ANY);

	transactionList = new TransactionsListPanel(transactionsPanelPlaceholder, wxID_ANY);
	transactionList->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this);
	transactionList->OnSplitTransaction = std::bind(&MainFrame::SplitTransaction, this);
	transactionList->OnPeriodChanged = std::bind(&MainFrame::UpdateStatus, this);

	homePanel = new HomePanel(homePanelPlaceholder, wxID_ANY);
	reportPanel = new ReportPanel(reportPanelPlaceholder, wxID_ANY);

	budgetsList = new BudgetsListPanel(budgetsPanelPlaceholder, wxID_ANY);
	budgetsList->OnEditBudget = std::bind(&MainFrame::EditBudget, this);

	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	transactionsPanelPlaceholder->SetSizer(boxSizer);

	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(homePanel, 1, wxEXPAND | wxALL, 0);
	homePanelPlaceholder->SetSizer(boxSizer);

	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);
	reportPanelPlaceholder->SetSizer(boxSizer);

	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(budgetsList, 1, wxEXPAND | wxALL, 0);
	budgetsPanelPlaceholder->SetSizer(boxSizer);

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);

	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);

	menuAccounts->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, ID_ADD_ACCOUNT);

	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransaction, this, ID_ADD_TRANSACTION);
	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnDuplicateTransaction, this, ID_DUPLICATE_TRANSACTION);
	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSplitTransaction, this, ID_SPLIT_TRANSACTION);

	menuBudgets->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddBudget, this, ID_ADD_BUDGET);

	treeMenu->Update();
}

MainFrame::~MainFrame() 
{
	Settings::GetInstance().SetWindowWidth(this->GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(this->GetSize().GetHeight());

	Settings::GetInstance().Save();
	
	delete treeMenu;
	delete transactionList;
	delete homePanel;
	delete reportPanel;
}

void MainFrame::UpdateTransactionList(TreeMenuItemTypes type, Account *account) 
{
	if (type == TreeMenuItemTypes::MenuAccount) {
		transactionList->SetType(type);
		transactionList->SetAccount(account);
		transactionList->Update();

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
	else if (type == TreeMenuItemTypes::MenuExpenses) {
		transactionList->SetType(type);
		transactionList->Update();

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Expenses: %.2f", amount));
	}
	else if (type == TreeMenuItemTypes::MenuReceipts) {
		transactionList->SetType(type);
		transactionList->Update();

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Receipts: %.2f", amount));
	}
	else if (type == TreeMenuItemTypes::MenuDeposits) {
		transactionList->SetType(type);
		transactionList->Update();

		float amount = transactionList->GetBalance();
		SetStatusText(wxString::Format("Deposits: %.2f", amount));
	}
}

void MainFrame::UpdateStatus() {
	/*wxTreeItemId itemId = treeMenu->GetSelection();

	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(TreeMenuItemTypes::MenuAccount, account.get());
		}
		else if (item->type == TreeMenuItemTypes::MenuExpenses) {
			UpdateTransactionList(TreeMenuItemTypes::MenuExpenses, 0);
		}
		else if (item->type == TreeMenuItemTypes::MenuReceipts) {
			UpdateTransactionList(TreeMenuItemTypes::MenuReceipts, 0);
		}
		else if (item->type == TreeMenuItemTypes::MenuDeposits) {
			UpdateTransactionList(TreeMenuItemTypes::MenuDeposits, 0);
		}
	}*/
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

void MainFrame::HideAllPanels() {
	rightPanelSizer->Detach(transactionsPanelPlaceholder);
	rightPanelSizer->Detach(homePanelPlaceholder);
	rightPanelSizer->Detach(reportPanelPlaceholder);
	rightPanelSizer->Detach(budgetsPanelPlaceholder);

	transactionsPanelPlaceholder->Hide();
	homePanelPlaceholder->Hide();
	reportPanelPlaceholder->Hide();
	budgetsPanelPlaceholder->Hide();
}

void MainFrame::OnTreeMenuAccountSelect(std::shared_ptr<Account> account) {
	HideAllPanels();

	rightPanelSizer->Add(transactionsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
	transactionsPanelPlaceholder->Show();
	rightPanelSizer->Layout();

	UpdateTransactionList(TreeMenuItemTypes::MenuAccount, account.get());
}

void MainFrame::OnTreeMenuReportSelect(std::shared_ptr<Report> report) {
	HideAllPanels();

	rightPanelSizer->Add(reportPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
	reportPanelPlaceholder->Show();
	rightPanelSizer->Layout();

	reportPanel->Update();
}

void MainFrame::OnTreeMenuHomeSelect() {
	HideAllPanels();

	rightPanelSizer->Add(homePanelPlaceholder, 1, wxEXPAND | wxALL, 0);
	homePanelPlaceholder->Show();
	rightPanelSizer->Layout();

	homePanel->Update();
}

void MainFrame::OnTreeMenuBudgetsSelect() {
	HideAllPanels();

	rightPanelSizer->Add(budgetsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
	budgetsPanelPlaceholder->Show();
	rightPanelSizer->Layout();

	budgetsList->Update();
}

/*void MainFrame::OnTreeItemSelect(wxTreeEvent &event)
{
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {		
		rightPanelSizer->Detach(transactionsPanelPlaceholder);
		rightPanelSizer->Detach(homePanelPlaceholder);
		rightPanelSizer->Detach(reportPanelPlaceholder);
		rightPanelSizer->Detach(budgetsPanelPlaceholder);

		transactionsPanelPlaceholder->Hide();
		homePanelPlaceholder->Hide();
		reportPanelPlaceholder->Hide();
		budgetsPanelPlaceholder->Hide();

		if (item->type == TreeMenuItemTypes::MenuAccount) {
			rightPanelSizer->Add(transactionsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			transactionsPanelPlaceholder->Show();			

			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(TreeMenuItemTypes::MenuAccount, account.get());

			selectedAccountId = account->id;
		}
		else if (item->type == TreeMenuItemTypes::MenuExpenses) {
			rightPanelSizer->Add(transactionsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			transactionsPanelPlaceholder->Show();
			
			UpdateTransactionList(TreeMenuItemTypes::MenuExpenses, 0);
		}
		else if (item->type == TreeMenuItemTypes::MenuReceipts) {
			rightPanelSizer->Add(transactionsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			transactionsPanelPlaceholder->Show();
			
			UpdateTransactionList(TreeMenuItemTypes::MenuReceipts, 0);
		}
		else if (item->type == TreeMenuItemTypes::MenuDeposits) {
			rightPanelSizer->Add(transactionsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			transactionsPanelPlaceholder->Show();
			
			UpdateTransactionList(TreeMenuItemTypes::MenuDeposits, 0);
		}
		else if (item->type == TreeMenuItemTypes::MenuReport) {
			rightPanelSizer->Add(reportPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			reportPanelPlaceholder->Show();

			reportPanel->Update();
		}
		else if (item->type == TreeMenuItemTypes::MenuBudget) {
			rightPanelSizer->Add(budgetsPanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			budgetsPanelPlaceholder->Show();

			budgetsList->Update();
		}
		else {
			rightPanelSizer->Add(homePanelPlaceholder, 1, wxEXPAND | wxALL, 0);
			homePanelPlaceholder->Show();

			homePanel->Update();
		}

		rightPanelSizer->Layout();
	}
}*/

void MainFrame::OnAddTransaction(wxCommandEvent &event) {
	AddTransaction();
}

void MainFrame::OnDuplicateTransaction(wxCommandEvent &event) {
	transactionList->DublicateTransaction();
}

void MainFrame::OnSplitTransaction(wxCommandEvent &event) {
	SplitTransaction();
}

void MainFrame::OnOpenNewTab(wxCommandEvent &event) {

}

void MainFrame::AddTransaction() {
	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);
	
	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();

	auto account = treeMenu->GetAccount();
	auto transaction = make_shared<Transaction>();

	transactionFrame->SetTransaction(transaction);
	transactionFrame->SetAccount(account);

	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
}

void MainFrame::EditTransaction() {
	auto transaction = transactionList->GetTransaction();

	if (transaction) {
		transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 350);

		transactionFrame->Show(true);
		transactionFrame->CenterOnParent();

		transactionFrame->SetTransaction(transaction);
		transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
	}
}

void MainFrame::SplitTransaction() {
	auto transaction = transactionList->GetTransaction();

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

	transactionList->Update();
	/*wxTreeItemId itemId = treeMenu->GetSelection();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(TreeMenuItemTypes::MenuAccount, account.get());
		} else if (item->type == TreeMenuItemTypes::MenuExpenses) {
			UpdateTransactionList(TreeMenuItemTypes::MenuExpenses, 0);		
		} else if (item->type == TreeMenuItemTypes::MenuReceipts) {
			UpdateTransactionList(TreeMenuItemTypes::MenuReceipts, 0);
		} else if (item->type == TreeMenuItemTypes::MenuDeposits) {
			UpdateTransactionList(TreeMenuItemTypes::MenuDeposits, 0);
		}
	}*/

	homePanel->Update();
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
	auto budget = budgetsList->GetBudget();

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
	budgetsList->Update();
}