#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	wxBoxSizer *mainSizer= new wxBoxSizer(wxVERTICAL);

	scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	scrolledWindow->SetScrollRate(5, 5);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	leftPanel = new wxPanel(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

	balancePanel = new DashboardBalancePanel(leftPanel);
	leftSizer->Add(balancePanel, 0, wxEXPAND | wxALL, 5);

	accountsPanel = new DashboardAccountsPanel(leftPanel);
	leftSizer->Add(accountsPanel, 0, wxEXPAND | wxALL, 5);

	expensesPanel = new DashboardExpensesPanel(leftPanel);
	leftSizer->Add(expensesPanel, 0, wxEXPAND | wxALL, 5);

	leftPanel->SetSizer(leftSizer);
	leftPanel->Layout();

	leftSizer->Fit(leftPanel);
	horizontalSizer->Add(leftPanel, 4, wxEXPAND | wxALL, 10);

	horizontalSizer->Add(0, 0, 1, wxEXPAND, 0);

	rightPanel = new wxPanel(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

	schedulersPanel = new DashboardSchedulersPanel(rightPanel);
	rightSizer->Add(schedulersPanel, 0, wxEXPAND | wxALL, 5);

	budgetsPanel = new DashboardBudgetsPanel(rightPanel);
	rightSizer->Add(budgetsPanel, 0, wxEXPAND | wxALL, 5);

	goalsPanel = new DashboardGoalsPanel(rightPanel);
	rightSizer->Add(goalsPanel, 0, wxEXPAND | wxALL, 5);

	debtsPanel = new DashboardDebtsPanel(rightPanel);
	rightSizer->Add(debtsPanel, 0, wxEXPAND | wxALL, 5);

	rightPanel->SetSizer(rightSizer);
	rightPanel->Layout();
	rightSizer->Fit(rightPanel);

	horizontalSizer->Add(rightPanel, 6, wxEXPAND | wxALL, 10);

	scrolledWindow->SetSizer(horizontalSizer);
	scrolledWindow->Layout();

	horizontalSizer->Fit(scrolledWindow);
	mainSizer->Add(scrolledWindow, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

void DashboardPanel::Update() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();
	wxDateTime today = wxDateTime::Now();
	wxDateTime month = wxDateTime::Now().Add(wxDateSpan(0, 0, 0, 30));

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();
	
	float ownFunds = _context.GetAccountingService().GetBalance();
	float creditFunds = _context.GetAccountingService().GetCredit();
	float totalBalance = ownFunds + creditFunds;

	auto currency = _context.GetCurrenciesRepository().GetBaseCurrency();

	balancePanel->SetBalance({ *currency, totalBalance }, { *currency, ownFunds }, { *currency, creditFunds });

	auto accounts = _context.GetAccountsService().GetByType(AccountType::Deposit);

	if (accounts.size() > 0) {
		accountsPanel->SetAccounts(accounts);
		accountsPanel->Show();
	}
	else {
		accountsPanel->Hide();
	}

	auto expenses = _context.GetAccountsService().GetExpenses(fromDate, toDate);

	if (expenses.size() > 0) {
		float totalExpenses = _context.GetAccountingService().GetExpenses(fromDate, toDate);

		expensesPanel->SetTotal({ *currency, totalExpenses });
		expensesPanel->SetExpenses(expenses);
		expensesPanel->Show();
	}
	else {
		expensesPanel->Hide();
	}
	
	auto schedulers = _context.GetSchedulersService().GetByPeriod(today, month);	

	if (schedulers.size() > 0) {
		schedulersPanel->SetSchedulers(schedulers);
		schedulersPanel->Show();
	}
	else {
		schedulersPanel->Hide();
	}

	auto budgets = _context.GetBudgetsService().GetAll();

	if (budgets.size() > 0) {
		budgetsPanel->SetBudgets(budgets);
		budgetsPanel->Show();
	}
	else {
		budgetsPanel->Hide();
	}

	auto goals = _context.GetGoalsService().GetAll();

	if (goals.size() > 0) {
		goalsPanel->SetGoals(goals);
		goalsPanel->Show();
	}
	else {
		goalsPanel->Hide();
	}

	auto debts = _context.GetAccountsService().GetDebts();

	if (debts.size() > 0) {
		debtsPanel->SetDebts(debts);
		debtsPanel->Show();
	}
	else {
		debtsPanel->Hide();
	}

	this->Layout();
}