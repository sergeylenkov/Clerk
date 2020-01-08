#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{	
	DataHelper::GetInstance().CreateAccountsImageList();

	this->SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	toolbar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	toolbar->SetBackgroundColour(wxColour(255, 255, 255));

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	addTransactionButton = new DropDownButton(toolbar, wxID_ANY, wxT("Add Transaction"), wxDefaultPosition, wxSize(-1, 34));
	addTransactionButton->SetBackgroundColour(wxColour(255, 255, 255));

	addTransactionButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddQuickTransaction, this);

	horizontalSizer->Add(addTransactionButton, 0, wxALL, 5);

	toolbar->SetSizer(horizontalSizer);
	toolbar->Layout();

	mainSizer->Add(toolbar, 0, wxEXPAND, 0);

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	splitter->SetSashGravity(0.5);
	splitter->SetMinimumPaneSize(300);

	mainSizer->Add(splitter, 1, wxEXPAND, 5);

	wxPanel *splitterLeftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	splitterLeftPanel->SetBackgroundColour(wxColour(245, 245, 245, 1));

	wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new TreeMenu(splitterLeftPanel, wxID_ANY);

	treeMenu->OnAccountSelect = std::bind(&MainFrame::OnTreeMenuAccountSelect, this, std::placeholders::_1);
	treeMenu->OnReportSelect = std::bind(&MainFrame::OnTreeMenuReportSelect, this, std::placeholders::_1);
	treeMenu->OnDashboardSelect = std::bind(&MainFrame::OnTreeMenuDashboardSelect, this);
	treeMenu->OnBudgetsSelect = std::bind(&MainFrame::OnTreeMenuBudgetsSelect, this);
	treeMenu->OnGoalsSelect = std::bind(&MainFrame::OnTreeMenuGoalsSelect, this);
	treeMenu->OnSchedulersSelect = std::bind(&MainFrame::OnTreeMenuSchedulersSelect, this);
	treeMenu->OnTrashSelect = std::bind(&MainFrame::OnTreeMenuTrashSelect, this);
	treeMenu->OnTagsSelect = std::bind(&MainFrame::OnTreeMenuTagsSelect, this);
	treeMenu->OnAlertsSelect = std::bind(&MainFrame::OnTreeMenuAlertsSelect, this);
	treeMenu->OnAccountsSelect = std::bind(&MainFrame::OnTreeMenuAccountsSelect, this, std::placeholders::_1);
	treeMenu->OnAddAccount = std::bind(&MainFrame::OnTreeMenuAddAccount, this, std::placeholders::_1);
	treeMenu->OnEditAccount = std::bind(&MainFrame::EditAccount, this, std::placeholders::_1);
	treeMenu->OnArchiveAccount = std::bind(&MainFrame::DeleteAccount, this, std::placeholders::_1);
	treeMenu->OnRestoreAccount = std::bind(&MainFrame::RestoreAccount, this, std::placeholders::_1);
	treeMenu->OnAddTransactionForAccount = std::bind(&MainFrame::OnTreeMenuAddTransactionForAccount, this, std::placeholders::_1);
	treeMenu->OnAddTransaction = std::bind(&MainFrame::OnTreeMenuAddTransaction, this, std::placeholders::_1);
	treeMenu->OnAddBudget = std::bind(&MainFrame::AddBudget, this);
	treeMenu->OnAddScheduler = std::bind(&MainFrame::AddScheduler, this);
	treeMenu->OnAddGoal = std::bind(&MainFrame::AddGoal, this);
	treeMenu->OnAddAlert = std::bind(&MainFrame::AddAlert, this);
	treeMenu->OnNewTab = std::bind(&MainFrame::AddTab, this, std::placeholders::_1, std::placeholders::_2);
	treeMenu->OnEmptyTrash = std::bind(&MainFrame::OnEmptyTrash, this);

	boxSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);		

	wxPanel *splitterRightPanel = new wxPanel(splitter, wxID_ANY);	

	rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	tabsPanel = new TabsPanel(splitterRightPanel, wxID_ANY);

	tabsPanel->OnAddTransaction = std::bind(&MainFrame::OnAddTransactionFromList, this);
	tabsPanel->OnCopyTransaction = std::bind(&MainFrame::CopyTransaction, this, std::placeholders::_1);
	tabsPanel->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this, std::placeholders::_1);
	tabsPanel->OnSplitTransaction = std::bind(&MainFrame::SplitTransaction, this, std::placeholders::_1);
	tabsPanel->OnAddBudget = std::bind(&MainFrame::AddBudget, this);
	tabsPanel->OnEditBudget = std::bind(&MainFrame::EditBudget, this, std::placeholders::_1);
	tabsPanel->OnAddScheduler = std::bind(&MainFrame::AddScheduler, this);
	tabsPanel->OnEditScheduler = std::bind(&MainFrame::EditScheduler, this, std::placeholders::_1);
	tabsPanel->OnAddGoal = std::bind(&MainFrame::AddGoal, this);
	tabsPanel->OnEditGoal = std::bind(&MainFrame::EditGoal, this, std::placeholders::_1);
	tabsPanel->OnAddAlert = std::bind(&MainFrame::AddAlert, this);
	tabsPanel->OnEditAlert = std::bind(&MainFrame::EditAlert, this, std::placeholders::_1);

	rightPanelSizer->Add(tabsPanel, 1, wxEXPAND | wxALL, 0);
	rightPanelSizer->Layout();

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);

	statusbar = new Statusbar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));	
	mainSizer->Add(statusbar, 0, wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	CreateMainMenu();
	CreateDropdownMenu();

	treeMenu->Update();
	treeMenu->RestoreState();

	tabsPanel->RestoreTabs();
	
	UpdateStatus();

	std::thread([=]()
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));		
		this->GetEventHandler()->CallAfter(&MainFrame::CheckSchedulers);
	}).detach();

	std::thread([=]()
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		this->GetEventHandler()->CallAfter(&MainFrame::CheckAlerts);
	}).detach();

	if (Settings::GetInstance().IsLoadExchangeRates()) {
		statusbar->SetExchangeRates("Updating...");		

		std::thread([=]()
		{
			UpdateExchangeRates();
			DataHelper::GetInstance().ReloadExchangeRate();

			std::this_thread::sleep_for(std::chrono::seconds(3));

			this->GetEventHandler()->CallAfter(&MainFrame::UpdateStatus);
		}).detach();
	}
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
	auto transactions = DataHelper::GetInstance().GetRecentTransactions();

	wxMenu *menuFile = new wxMenu();
	wxMenu *menuHelp = new wxMenu();

	menuHelp->Append(static_cast<int>(MainMenuTypes::About), "&About...");

	if (transactions.size() == 0) {
		menuFile->Append(static_cast<int>(MainMenuTypes::AddTransaction), wxT("New Transaction...\tCtrl+T"));
		menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddQuickTransaction, this, static_cast<int>(MainMenuTypes::AddTransaction));
	}
	else {
		wxMenu *menuTransaction = new wxMenu();
		menuFile->AppendSubMenu(menuTransaction, wxT("New Transaction"));

		menuTransaction->Append(static_cast<int>(MainMenuTypes::AddTransaction), wxT("New Transaction...\tCtrl+T"));
		menuTransaction->AppendSeparator();

		for (auto &transaction : transactions)
		{
			menuTransaction->Append(transaction->id, wxString::Format("%s › %s (%s)", *transaction->fromAccount->name, *transaction->toAccount->name, transaction->GetTagsString()));
		}

		menuTransaction->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransactionFromMainMenu, this);
	}

	menuFile->AppendSeparator();
	menuFile->Append(static_cast<int>(MainMenuTypes::AddAccount), wxT("New Account..."));
	menuFile->Append(static_cast<int>(MainMenuTypes::AddBudget), wxT("New Budget..."));
	menuFile->Append(static_cast<int>(MainMenuTypes::AddGoal), wxT("New Goal..."));
	menuFile->Append(static_cast<int>(MainMenuTypes::AddScheduler), wxT("New Scheduler..."));
	menuFile->Append(static_cast<int>(MainMenuTypes::AddAlert), wxT("New Alert..."));
	menuFile->AppendSeparator();
	menuFile->Append(static_cast<int>(MainMenuTypes::Preferences), "Preferences...\tCtrl+P");
	menuFile->AppendSeparator();
	menuFile->Append(static_cast<int>(MainMenuTypes::Exit), "E&xit\tCtrl+W");

	wxMenuBar *menuBar = new wxMenuBar();

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");	

	SetMenuBar(menuBar);

	menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, static_cast<int>(MainMenuTypes::About));
	
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, static_cast<int>(MainMenuTypes::AddAccount));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddBudget, this, static_cast<int>(MainMenuTypes::AddBudget));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddGoal, this, static_cast<int>(MainMenuTypes::AddGoal));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddScheduler, this, static_cast<int>(MainMenuTypes::AddScheduler));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnPreferences, this, static_cast<int>(MainMenuTypes::Preferences));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, static_cast<int>(MainMenuTypes::Exit));
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAlert, this, static_cast<int>(MainMenuTypes::AddAlert));
}

void MainFrame::CreateDropdownMenu() {
	wxMenu *menu = addTransactionButton->GetMenu();
	auto transactions = DataHelper::GetInstance().GetRecentTransactions();

	for (auto &transaction : transactions)
	{
		menu->Append(transaction->id, wxString::Format("%s › %s (%s)", *transaction->fromAccount->name, *transaction->toAccount->name, transaction->GetTagsString()));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransactionFromMainMenu, this);
}

void MainFrame::UpdateStatus() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();

	float receipts = 0;
	float expenses = 0;
	float balance = 0;

	for (auto &account : DataHelper::GetInstance().GetAccountsByType(AccountType::Receipt))
	{
		float amount = DataHelper::GetInstance().GetReceipts(*account, &fromDate, &toDate);
		amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);

		receipts = receipts + amount;
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Expens))
	{
		float amount = DataHelper::GetInstance().GetExpenses(*account, &fromDate, &toDate);		
		amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
		
		expenses = expenses + amount;
	}
	
	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Debt))
	{
		float amount = DataHelper::GetInstance().GetExpenses(*account, &fromDate, &toDate);
		amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);

		expenses = expenses + amount;
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Deposit))
	{
		if (account->creditLimit == 0) {			
			float amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, account->balance);
			balance = balance + amount;
		}
	}

	statusbar->SetPeriod(wxDateTime::Now().Format("%B"));
	statusbar->SetRecepipts(wxNumberFormatter::ToString(receipts, 2));
	statusbar->SetExpenses(wxNumberFormatter::ToString(expenses, 2));
	statusbar->SetBalance(wxNumberFormatter::ToString(balance, 2));

	wxString rates("");

	auto exchangeRates = DataHelper::GetInstance().GetExchangeRates();
	auto settingsRates = Settings::GetInstance().GetSelectedExchangeRates();

	for (int id : settingsRates) {
		if (exchangeRates[std::make_pair(id, baseCurrencyId)]) {
			float rate = exchangeRates[std::make_pair(id, baseCurrencyId)];
			Currency currecy(id);

			rates = rates + wxNumberFormatter::ToString(rate, 2) + wxT(" ") + *currecy.shortName.get() + wxT("  ");
		}
	}
	
	statusbar->SetExchangeRates(rates);

	statusbar->Layout();
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	AboutDialog *aboutDialog = new AboutDialog(this, wxT("About"), 0, 0, 250, 340);

	aboutDialog->Show(true);
	aboutDialog->CenterOnParent();
}

void MainFrame::OnPreferences(wxCommandEvent &event)
{
	PreferencesDialog *preferencesDialog = new PreferencesDialog(this, wxT("Preferences"), 0, 0, 400, 300);

	preferencesDialog->Show(true);
	preferencesDialog->CenterOnParent();
}

void MainFrame::OnAddAccount(wxCommandEvent &event) {
	AddAccount(AccountType::Deposit);
}

void MainFrame::OnTreeMenuAccountSelect(std::shared_ptr<Account> account) {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Account, account->id)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Account, account->id);
	}
	else {		
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Account, account);
	}
}

void MainFrame::OnTreeMenuReportSelect(std::shared_ptr<Report> report) {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Report, report->id)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Report, report->id);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Report, report);
	}
}

void MainFrame::OnTreeMenuDashboardSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Dashboard)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Dashboard);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Dashboard, nullptr);
	}
}

void MainFrame::OnTreeMenuBudgetsSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Budgets)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Budgets);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Budgets, nullptr);
	}
}

void MainFrame::OnTreeMenuGoalsSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Goals)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Goals);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Goals, nullptr);
	}
}

void MainFrame::OnTreeMenuSchedulersSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Schedulers)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Schedulers);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Schedulers, nullptr);
	}
}

void MainFrame::OnTreeMenuTrashSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Trash)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Trash);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Trash, nullptr);
	}
}

void MainFrame::OnTreeMenuTagsSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Tags)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Tags);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Tags, nullptr);
	}
}

void MainFrame::OnTreeMenuAlertsSelect() {
	if (tabsPanel->IsTabExists(TreeMenuItemTypes::Alerts)) {
		tabsPanel->SelectTab(TreeMenuItemTypes::Alerts);
	}
	else {
		tabsPanel->UpdateCurrentTab(TreeMenuItemTypes::Alerts, nullptr);
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

void MainFrame::OnTreeMenuAddAccount(TreeMenuItemTypes type) {
	if (type == TreeMenuItemTypes::Receipts) {
		AddAccount(AccountType::Receipt);
	}
	else if (type == TreeMenuItemTypes::Expenses) {
		AddAccount(AccountType::Expens);
	}
	else if (type == TreeMenuItemTypes::Debt) {
		AddAccount(AccountType::Debt);
	}
	else {
		AddAccount(AccountType::Deposit);
	}
}

void MainFrame::OnTreeMenuAddTransactionForAccount(std::shared_ptr<Account> account) {
	auto transaction = make_shared<Transaction>();

	if (account) {
		if (account->type == AccountType::Receipt || account->type == AccountType::Deposit || account->type == AccountType::Virtual) {
			transaction->fromAccount = make_shared<Account>(account->id);
			transaction->toAccount = make_shared<Account>(DataHelper::GetInstance().GetPairAccountId(*account));
		}

		if (account->type == AccountType::Expens || account->type == AccountType::Debt) {
			transaction->toAccount = make_shared<Account>(account->id);
			transaction->fromAccount = make_shared<Account>(DataHelper::GetInstance().GetPairAccountId(*account));
		}
	}

	EditTransaction(transaction);
}

void MainFrame::OnTreeMenuAddTransaction(std::shared_ptr<Transaction> transaction) {
	EditTransaction(transaction);	
}

void MainFrame::OnAddQuickTransaction(wxCommandEvent &even) {
	AddTransaction();
}

void MainFrame::OnDuplicateTransaction(wxCommandEvent &event) {
	auto transaction = tabsPanel->GetSelectedTransaction();

	if (transaction) {
		Transaction *copy = new Transaction();

		copy->fromAccount = transaction->fromAccount;
		copy->toAccount = transaction->toAccount;
		copy->fromAmount = 0;
		copy->toAmount = 0;
		copy->note = transaction->note;
		copy->tags = transaction->tags;
		copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

		copy->Save();

		delete copy;

		UpdateUIData();
	}	
}

void MainFrame::OnAddTransactionFromMainMenu(wxCommandEvent &event) {
	int itemId = event.GetId();

	if (itemId == 0) {
		auto transaction = make_shared<Transaction>();
		ShowTransactionDialog(transaction);
	}
	else {
		auto transaction = make_shared<Transaction>(itemId);
		transaction->fromAmount = 0;
		transaction->toAmount = 0;

		CopyTransaction(transaction);
	}
}

void MainFrame::OnAddTransactionFromList() {
	AddTransaction();
}

void MainFrame::OnSplitTransaction(wxCommandEvent &event) {
	auto transaction = tabsPanel->GetSelectedTransaction();

	if (transaction) {
		SplitTransaction(transaction);
	}
}

void MainFrame::AddTransaction() {
	auto transaction = make_shared<Transaction>();
	ShowTransactionDialog(transaction);
}

void MainFrame::CopyTransaction(std::shared_ptr<Transaction> transaction) {
	auto copy = make_shared<Transaction>();

	copy->fromAccount = transaction->fromAccount;
	copy->toAccount = transaction->toAccount;
	copy->fromAmount = transaction->fromAmount;
	copy->toAmount = transaction->toAmount;
	copy->note = transaction->note;
	copy->tags = transaction->tags;
	copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

	ShowTransactionDialog(copy);
}

void MainFrame::EditTransaction(std::shared_ptr<Transaction> transaction) {
	ShowTransactionDialog(transaction);
}

void MainFrame::SplitTransaction(std::shared_ptr<Transaction> transaction) {
	TransactionDialog *transactionDialog = new TransactionDialog(this, wxT("Transaction"), 0, 0, 450, 350);
	
	transactionDialog->SetSplitTransaction(transaction);
	transactionDialog->OnClose = std::bind(&MainFrame::OnTransactionDialogClose, this);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void MainFrame::ShowTransactionDialog(std::shared_ptr<Transaction> transaction) {
	TransactionDialog *transactionDialog = new TransactionDialog(this, wxT("Transaction"), 0, 0, 450, 350);

	transactionDialog->SetTransaction(transaction);
	transactionDialog->OnClose = std::bind(&MainFrame::OnTransactionDialogClose, this);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void MainFrame::OnTransactionDialogClose() {
	UpdateUIData();
}

void MainFrame::AddAccount(AccountType type) {
	std::shared_ptr<Account> account = make_shared<Account>();
	account->type = type;

	ShowAccountDialog(account);
}

void MainFrame::EditAccount(std::shared_ptr<Account> account) {
	ShowAccountDialog(account);
}

void MainFrame::DeleteAccount(std::shared_ptr<Account> account) {
	account->Delete();

	treeMenu->Update();
	treeMenu->RestoreState();
}

void MainFrame::RestoreAccount(std::shared_ptr<Account> account) {
	account->Restore();

	treeMenu->Update();
	treeMenu->RestoreState();
}

void MainFrame::ShowAccountDialog(std::shared_ptr<Account> account) {
	AccountDialog *accountDialog = new AccountDialog(this, wxT("Account"), 0, 0, 340, 400);

	accountDialog->SetAccount(account);
	accountDialog->OnClose = std::bind(&MainFrame::OnAccountDialogClose, this);

	accountDialog->Show(true);
	accountDialog->CenterOnParent();
}

void MainFrame::OnAccountDialogClose() {
	treeMenu->Update();
	treeMenu->RestoreState();
}

void MainFrame::OnAddBudget(wxCommandEvent &event) {
	AddBudget();
}

void MainFrame::AddBudget() {
	ShowBudgetDialog(make_shared<Budget>());
}

void MainFrame::EditBudget(std::shared_ptr<Budget> budget) {
	ShowBudgetDialog(budget);
}

void MainFrame::ShowBudgetDialog(std::shared_ptr<Budget> budget) {
	BudgetDialog *budgetDialog = new BudgetDialog(this, wxT("Budget"), 0, 0, 340, 400);

	budgetDialog->SetBudget(budget);
	budgetDialog->OnClose = std::bind(&MainFrame::OnBudgetDialogClose, this);

	budgetDialog->Show(true);
	budgetDialog->CenterOnParent();
}

void MainFrame::OnBudgetDialogClose() {
	tabsPanel->Update();
}

void MainFrame::OnAddScheduler(wxCommandEvent &event) {
	AddScheduler();
}

void MainFrame::AddScheduler() {
	ShowSchedulerDialog(make_shared<Scheduler>());
}

void MainFrame::EditScheduler(std::shared_ptr<Scheduler> scheduler) {
	ShowSchedulerDialog(scheduler);
}

void MainFrame::ShowSchedulerDialog(std::shared_ptr<Scheduler> scheduler) {
	SchedulerDialog *schedulerDialog = new SchedulerDialog(this, wxT("Scheduler"), 0, 0, 450, 480);

	schedulerDialog->SetScheduler(scheduler);
	schedulerDialog->OnClose = std::bind(&MainFrame::OnSchedulerClose, this);

	schedulerDialog->Show(true);
	schedulerDialog->CenterOnParent();
}

void MainFrame::OnSchedulerClose() {
	tabsPanel->Update();
}

void MainFrame::OnAddGoal(wxCommandEvent &event) {
	AddGoal();
}

void MainFrame::AddGoal() {
	ShowGoalDialog(make_shared<Goal>());
}

void MainFrame::EditGoal(std::shared_ptr<Goal> goal) {
	ShowGoalDialog(goal);
}

void MainFrame::ShowGoalDialog(std::shared_ptr<Goal> goal) {
	GoalDialog *goalDialog = new GoalDialog(this, wxT("Goal"), 0, 0, 340, 400);

	goalDialog->SetGoal(goal);
	goalDialog->OnClose = std::bind(&MainFrame::OnGoalClose, this);

	goalDialog->Show(true);
	goalDialog->CenterOnParent();
}

void MainFrame::OnGoalClose() {
	tabsPanel->Update();
}

void MainFrame::OnAddAlert(wxCommandEvent &event) {
	AddAlert();
}

void MainFrame::AddAlert() {
	ShowAlertDialog(make_shared<Alert>());
}

void MainFrame::EditAlert(std::shared_ptr<Alert> alert) {
	ShowAlertDialog(alert);
}

void MainFrame::ShowAlertDialog(std::shared_ptr<Alert> alert) {
	AlertDialog *alertDialog = new AlertDialog(this, wxT("Alert"), 0, 0, 340, 400);

	alertDialog->SetAlert(alert);
	alertDialog->OnClose = std::bind(&MainFrame::OnAlertClose, this);

	alertDialog->Show(true);
	alertDialog->CenterOnParent();
}

void MainFrame::OnAlertClose() {
	tabsPanel->Update();
}

void MainFrame::AddTab(TreeMenuItemTypes type, shared_ptr<void> object) {
	tabsPanel->AddTab(type, object);
}

void MainFrame::CheckSchedulers() {
	wxDateTime today = wxDateTime::Now();
	today.ResetTime();

	std::vector<shared_ptr<Scheduler>> schedulers;

	for (auto &scheduler : DataHelper::GetInstance().GetSchedulers())
	{
		if (scheduler->active && (today.IsEqualTo(*scheduler->nextDate) || today.IsLaterThan(*scheduler->nextDate))) {
			schedulers.push_back(scheduler);
		}
	}

	if (schedulers.size() > 0) {
		SchedulersConfirmDialog *confirmDialog = new SchedulersConfirmDialog(this, wxT("Schedulers"), 0, 0, 450, 400);

		confirmDialog->SetSchedulers(schedulers);
		confirmDialog->OnClose = std::bind(&MainFrame::OnSchedulersConfirmClose, this);

		confirmDialog->Show(true);
		confirmDialog->CenterOnParent();
	}
}

void MainFrame::OnSchedulersConfirmClose() {
	tabsPanel->Update();
}

void MainFrame::OnEmptyTrash() {
	DataHelper::GetInstance().EmptyTrash();
	treeMenu->UpdateTrashItem();
}

void MainFrame::UpdateExchangeRates() {
	CBRRatesLoader loader(DataHelper::GetInstance().Connection());
	loader.Load();
}

void MainFrame::UpdateUIData() {
	DataHelper::GetInstance().UpdateAccountsBalance();
	UpdateStatus();
	tabsPanel->Update();	
}

void MainFrame::CheckAlerts() {
	std::vector<shared_ptr<Alert>> alerts;

	for (auto &alert : DataHelper::GetInstance().GetAlerts())
	{
		if (alert->type == Alert::Type::Balance) {
			float total = 0;

			wxStringTokenizer tokenizer(*alert->accountIds, ",");

			while (tokenizer.HasMoreTokens())
			{
				wxString id = tokenizer.GetNextToken().Trim(true).Trim(false);
				Account account(wxAtoi(id));

				float balance = DataHelper::GetInstance().GetBalance(account);
				total = total + balance;
			}

			if (alert->condition == Alert::Condition::Equal && total == alert->amount) {
				alerts.push_back(alert);
			}

			if (alert->condition == Alert::Condition::Less && total < alert->amount) {
				alerts.push_back(alert);
			}

			if (alert->condition == Alert::Condition::More && total > alert->amount) {
				alerts.push_back(alert);
			}
		}
	}

	if (alerts.size() > 0) {
		AlertsConfirmDialog *confirmDialog = new AlertsConfirmDialog(this, wxT("Alerts"), 0, 0, 350, 400);

		confirmDialog->SetAlerts(alerts);

		confirmDialog->Show(true);
		confirmDialog->CenterOnParent();
	}
}