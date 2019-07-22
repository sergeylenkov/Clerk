#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	wxBoxSizer *mainSizer= new wxBoxSizer(wxVERTICAL);

	scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	scrolledWindow->SetScrollRate(5, 5);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	leftPanel = new wxPanel(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

	balancePanel = new DashboardBalancePanel(leftPanel, wxID_ANY);
	leftSizer->Add(balancePanel, 0, wxEXPAND | wxALL, 5);

	accountsPanel = new DashboardAccountsPanel(leftPanel, wxID_ANY);
	leftSizer->Add(accountsPanel, 0, wxEXPAND | wxALL, 5);

	expensesPanel = new DashboardExpensesPanel(leftPanel, wxID_ANY);
	leftSizer->Add(expensesPanel, 0, wxEXPAND | wxALL, 5);

	leftPanel->SetSizer(leftSizer);
	leftPanel->Layout();

	leftSizer->Fit(leftPanel);
	horizontalSizer->Add(leftPanel, 4, wxEXPAND | wxALL, 10);

	horizontalSizer->Add(0, 0, 1, wxEXPAND, 0);

	rightPanel = new wxPanel(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

	schedulersPanel = new DashboardSchedulersPanel(rightPanel, wxID_ANY);
	rightSizer->Add(schedulersPanel, 0, wxEXPAND | wxALL, 5);

	budgetsPanel = new DashboardBudgetsPanel(rightPanel, wxID_ANY);
	rightSizer->Add(budgetsPanel, 0, wxEXPAND | wxALL, 5);

	goalsPanel = new DashboardGoalsPanel(rightPanel, wxID_ANY);
	rightSizer->Add(goalsPanel, 0, wxEXPAND | wxALL, 5);

	debtsPanel = new DashboardDebtsPanel(rightPanel, wxID_ANY);
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
	
	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	std::vector<AccountValue> expenses;

	for (auto &account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Expens))
	{
		float amount = DataHelper::GetInstance().GetExpenses(*account, &fromDate, &toDate);

		if (amount > 0) {
			amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
			expenses.push_back({ account.get(), amount });
		}
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Debt))
	{
		float amount = DataHelper::GetInstance().GetExpenses(*account, &fromDate, &toDate);

		if (amount > 0) {
			amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
			expenses.push_back({ account.get(), amount });
		}
	}

	std::vector<AccountValue> accounts;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		accounts.push_back({ account.get(), account->balance });
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Virtual))
	{
		accounts.push_back({ account.get(), account->balance });
	}

	std::map<int, float> ownFundsDict;
	std::map<int, float> creditFoundsDict;
	float totalBalance = 0;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		float amount = account->balance;

		if (account->isCredit) {
			float currentAmount = account->creditLimit + amount;

			if (creditFoundsDict[account->currency->id]) {
				creditFoundsDict[account->currency->id] = creditFoundsDict[account->currency->id] + currentAmount;
			}
			else {
				creditFoundsDict[account->currency->id] = currentAmount;
			}
		}
		else {
			if (ownFundsDict[account->currency->id]) {
				ownFundsDict[account->currency->id] = ownFundsDict[account->currency->id] + amount;
			}
			else {
				ownFundsDict[account->currency->id] = amount;
			}

			totalBalance = totalBalance + DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
		}
	}

	std::vector<CurrencyValue> ownFunds;

	for (auto &fund : ownFundsDict) {
		ownFunds.push_back({ Currency(fund.first), fund.second });
	}

	std::vector<CurrencyValue> creditFunds;

	for (auto &fund : creditFoundsDict) {
		creditFunds.push_back({ Currency(fund.first), fund.second });
	}

	std::vector<std::shared_ptr<Account>> debts;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		if (account->isCredit) {
			debts.push_back(account);
		}
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Debt))
	{
		debts.push_back(account);
	}
	
	balancePanel->SetBalance({ Currency(baseCurrencyId), totalBalance }, ownFunds, creditFunds);

	if (accounts.size() > 0) {
		accountsPanel->SetAccounts(accounts);
		accountsPanel->Show();
	}
	else {
		accountsPanel->Hide();
	}

	if (expenses.size() > 0) {
		expensesPanel->SetExpenses(expenses);
		expensesPanel->Show();
	}
	else {
		expensesPanel->Hide();
	}

	auto schedulers = DataHelper::GetInstance().GetSchedulers(&today, &month);

	if (schedulers.size() > 0) {
		schedulersPanel->SetSchedulers(schedulers);
		schedulersPanel->Show();
	}
	else {
		schedulersPanel->Hide();
	}

	auto budgets = DataHelper::GetInstance().GetBudgets();

	if (budgets.size() > 0) {
		budgetsPanel->SetBudgets(budgets);
		budgetsPanel->Show();
	}
	else {
		budgetsPanel->Hide();
	}

	auto goals = DataHelper::GetInstance().GetGoals();

	if (goals.size() > 0) {
		goalsPanel->SetGoals(goals);
		goalsPanel->Show();
	}
	else {
		goalsPanel->Hide();
	}

	if (debts.size() > 0) {
		debtsPanel->SetDebts(debts);
		debtsPanel->Show();
	}
	else {
		debtsPanel->Hide();
	}

	this->Layout();
}