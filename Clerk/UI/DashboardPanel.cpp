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
	std::vector<StringValue> expenses;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Expens))
	{
		float amount = DataHelper::GetInstance().GetExpenses(account.get(), &fromDate, &toDate);

		if (amount > 0) {
			amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
			expenses.push_back({ *account->name, amount });
		}
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Debt))
	{
		float amount = DataHelper::GetInstance().GetExpenses(account.get(), &fromDate, &toDate);

		if (amount > 0) {
			amount = DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
			expenses.push_back({ *account->name, amount });
		}
	}

	std::vector<StringValue> accounts;	

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());
		accounts.push_back({ *account->name, amount });
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Virtual))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());
		accounts.push_back({ *account->name, amount });
	}

	std::map<wxString, float> ownFounds;
	std::map<wxString, float> creditFounds;
	float totalBalance = 0;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());

		if (account->creditLimit > 0) {
			float currentAmount = account->creditLimit + amount;

			if (creditFounds[*account->currency->shortName]) {
				creditFounds[*account->currency->shortName] = creditFounds[*account->currency->shortName] + currentAmount;
			}
			else {
				creditFounds[*account->currency->shortName] = currentAmount;
			}
		}
		else {
			if (ownFounds[*account->currency->shortName]) {
				ownFounds[*account->currency->shortName] = ownFounds[*account->currency->shortName] + amount;
			}
			else {
				ownFounds[*account->currency->shortName] = amount;
			}

			totalBalance = totalBalance + DataHelper::GetInstance().ConvertCurrency(account->currency->id, baseCurrencyId, amount);
		}
	}

	std::vector<std::shared_ptr<Account>> debts;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		if (account->creditLimit > 0) {
			debts.push_back(account);
		}
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountTypes::Debt))
	{
		debts.push_back(account);
	}
	
	balancePanel->SetBalance(totalBalance, ownFounds, creditFounds);
	schedulersPanel->SetSchedulers(DataHelper::GetInstance().GetSchedulers(&today, &month));
	budgetsPanel->SetBudgets(DataHelper::GetInstance().GetBudgets());
	expensesPanel->SetExpenses(expenses);
	goalsPanel->SetGoals(DataHelper::GetInstance().GetGoals());
	debtsPanel->SetDebts(debts);
	accountsPanel->SetAccounts(accounts);

	this->Layout();
}