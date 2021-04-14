#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	wxBoxSizer *mainSizer= new wxBoxSizer(wxVERTICAL);

	_scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_scrolledWindow->SetScrollRate(5, 5);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_leftPanel = new wxPanel(_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

	DashboardViewModel* viewModel = new DashboardViewModel(_context.GetAccountingService(), *_context.GetCurrenciesRepository().GetBaseCurrency());

	_balancePanel = new DashboardBalancePanel(_leftPanel);
	_balancePanel->SetViewModel(viewModel);

	leftSizer->Add(_balancePanel, 0, wxEXPAND | wxALL, 5);

	_accountsPanel = new DashboardAccountsPanel(_leftPanel);
	leftSizer->Add(_accountsPanel, 0, wxEXPAND | wxALL, 5);

	_expensesPanel = new DashboardExpensesPanel(_leftPanel);
	leftSizer->Add(_expensesPanel, 0, wxEXPAND | wxALL, 5);

	_leftPanel->SetSizer(leftSizer);
	_leftPanel->Layout();

	leftSizer->Fit(_leftPanel);
	horizontalSizer->Add(_leftPanel, 4, wxEXPAND | wxALL, 10);

	horizontalSizer->Add(0, 0, 1, wxEXPAND, 0);

	_rightPanel = new wxPanel(_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

	_schedulersPanel = new DashboardSchedulersPanel(_rightPanel);
	rightSizer->Add(_schedulersPanel, 0, wxEXPAND | wxALL, 5);

	_budgetsPanel = new DashboardBudgetsPanel(_rightPanel);
	rightSizer->Add(_budgetsPanel, 0, wxEXPAND | wxALL, 5);

	_goalsPanel = new DashboardGoalsPanel(_rightPanel);
	rightSizer->Add(_goalsPanel, 0, wxEXPAND | wxALL, 5);

	_debtsPanel = new DashboardDebtsPanel(_rightPanel);
	rightSizer->Add(_debtsPanel, 0, wxEXPAND | wxALL, 5);

	_rightPanel->SetSizer(rightSizer);
	_rightPanel->Layout();
	rightSizer->Fit(_rightPanel);

	horizontalSizer->Add(_rightPanel, 6, wxEXPAND | wxALL, 10);

	_scrolledWindow->SetSizer(horizontalSizer);
	_scrolledWindow->Layout();

	horizontalSizer->Fit(_scrolledWindow);
	mainSizer->Add(_scrolledWindow, 1, wxEXPAND | wxALL, 0);

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

	auto currency = _context.GetCurrenciesRepository().GetBaseCurrency();

	auto accounts = _context.GetAccountsService().GetByType(AccountType::Deposit);

	if (accounts.size() > 0) {
		_accountsPanel->SetAccounts(accounts);
		_accountsPanel->Show();
	}
	else {
		_accountsPanel->Hide();
	}

	auto expenses = _context.GetAccountsService().GetExpenses(fromDate, toDate);

	if (expenses.size() > 0) {
		float totalExpenses = _context.GetAccountingService().GetExpenses(fromDate, toDate);

		_expensesPanel->SetTotal({ *currency, totalExpenses });
		_expensesPanel->SetExpenses(expenses);
		_expensesPanel->Show();
	}
	else {
		_expensesPanel->Hide();
	}
	
	auto schedulers = _context.GetSchedulersService().GetByPeriod(today, month);	

	if (schedulers.size() > 0) {
		_schedulersPanel->SetSchedulers(schedulers);
		_schedulersPanel->Show();
	}
	else {
		_schedulersPanel->Hide();
	}

	auto budgets = _context.GetBudgetsService().GetAll();

	if (budgets.size() > 0) {
		_budgetsPanel->SetBudgets(budgets);
		_budgetsPanel->Show();
	}
	else {
		_budgetsPanel->Hide();
	}

	auto goals = _context.GetGoalsService().GetAll();

	if (goals.size() > 0) {
		_goalsPanel->SetGoals(goals);
		_goalsPanel->Show();
	}
	else {
		_goalsPanel->Hide();
	}

	auto debts = _context.GetAccountsService().GetDebts();

	if (debts.size() > 0) {
		_debtsPanel->SetDebts(debts);
		_debtsPanel->Show();
	}
	else {
		_debtsPanel->Hide();
	}

	this->Layout();
}