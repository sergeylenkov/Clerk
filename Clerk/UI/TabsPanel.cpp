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
		Settings::GetInstance().AddTab(panel->type, panel->id);
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
		} else if (tab.type == TreeMenuItemTypes::MenuReport) {
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
		|| type == TreeMenuItemTypes::MenuExpenses || type == TreeMenuItemTypes::MenuDeposits) {
		CreateAccountsPanel(tabIndex, (TreeMenuItemTypes)type);
	}
	else if (type == TreeMenuItemTypes::MenuHome) {
		CreateHomePanel(tabIndex);
	}
	else if (type == TreeMenuItemTypes::MenuBudgets) {
		CreateBudgetsPanel(tabIndex);
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

	transactionList->OnEditTransaction = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
	transactionList->OnSplitTransaction = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);
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
	transactionList->OnEditTransaction = std::bind(&TabsPanel::EditTransaction, this, std::placeholders::_1);
	transactionList->OnSplitTransaction = std::bind(&TabsPanel::SplitTransaction, this, std::placeholders::_1);
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

	notebook->SetPageText(tabIndex, name);
	UpdateTransactionList(transactionList, type, nullptr);
}

void TabsPanel::CreateHomePanel(int tabIndex) {
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

	notebook->SetPageText(tabIndex, wxT("Dashboard"));

	homePanel->Update();
}

void TabsPanel::CreateBudgetsPanel(int tabIndex) {
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

	notebook->SetPageText(tabIndex, wxT("Budgets"));

	budgetPanel->Update();
}

void TabsPanel::CreateReportPanel(int tabIndex, std::shared_ptr<Report> report) {
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

	notebook->SetPageText(tabIndex, *report->name);

	reportPanel->Update();
}

void TabsPanel::OnTabChanged(wxBookCtrlEvent &event) {
	int i = notebook->GetSelection();
	tabsPanels[i]->Update();

	UpdateStatus();
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	contextMenuTab = notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	wxMenu *menu = new wxMenu();

	wxMenuItem *leftItem = new wxMenuItem(menu, ID_TAB_MOVE_LEFT, wxT("Move to Left"));
	wxMenuItem *rightItem = new wxMenuItem(menu, ID_TAB_MOVE_RIGHT, wxT("Move to Right"));
	wxMenuItem *closeItem = new wxMenuItem(menu, ID_TAB_CLOSE, wxT("Close"));

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

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsPanel::OnTabMenuClose, this, ID_TAB_CLOSE);

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

	UpdateStatus();
}

void TabsPanel::EditTransaction(std::shared_ptr<Transaction> transaction) {
	if (transaction) {
		if (OnEditTransaction) {
			OnEditTransaction(transaction);
		}
	}
}

void TabsPanel::SplitTransaction(std::shared_ptr<Transaction> transaction) {
	if (transaction) {
		if (OnSplitTransaction) {
			OnSplitTransaction(transaction);
		}
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
			result = wxString::Format("%s: %.2f (%.2f %.2f) %s", static_cast<const char*>(name), account->creditLimit + amount, account->creditLimit, amount, static_cast<const char*>(account->currency->shortName->c_str()));
		}
		else {
			result = wxString::Format("%s: %.2f %s", static_cast<const char*>(name), amount, static_cast<const char*>(account->currency->shortName->c_str()));
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
	else if (currentPanel->type == TreeMenuItemTypes::MenuHome) {
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
		BudgetsListPanel *budgetsList = (BudgetsListPanel *)currentPanel;
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
	notebook->RemovePage(index);

	tabs.erase(tabs.begin() + index);
	tabsSizer.erase(tabsSizer.begin() + index);
	tabsPanels.erase(tabsPanels.begin() + index);
}