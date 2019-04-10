#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(DashboardPanel::OnPaint));

	paddingX = 20;
	paddingY = 20;

	color0 = wxColor(165, 210, 75);
	color50 = wxColor(250, 210, 50);
	color100 = wxColor(185, 25, 0);
}

DashboardPanel::~DashboardPanel() {
}

void DashboardPanel::Update() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	balance.clear();
	virtualBalance.clear();
	expenses.clear();
	budgets.clear();
	credits.clear();
	goals.clear();

	float sum = 0;

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());

		if (account->creditLimit > 0) {
			float currentAmount = account->creditLimit + amount;
			float remainPercent = abs(currentAmount / account->creditLimit) * 100.0;

			credits.push_back({ *account->name, wxNumberFormatter::ToString(account->creditLimit, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(amount, 2), remainPercent });
		}
		else {
			balance.push_back({ *account->name, wxNumberFormatter::ToString(amount, 2) });
			sum = sum + amount;
		}
	}

	totalBalance = wxNumberFormatter::ToString(sum, 2);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Virtual))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());
		virtualBalance.push_back({ *account->name, wxNumberFormatter::ToString(amount, 2) });
	}

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Credit))
	{
		float remainAmount = abs(DataHelper::GetInstance().GetBalance(account.get()));
		float amount = abs(DataHelper::GetInstance().GetAccountTotalExpense(account.get()));
		float currentAmount = DataHelper::GetInstance().GetAccountTotalReceipt(account.get());

		float remainPercent = (currentAmount / amount) * 100.0;

		credits.push_back({ *account->name, wxNumberFormatter::ToString(amount, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(remainAmount, 2), remainPercent });
	}

	vector<StringValue> values = DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate);

	std::sort(values.begin(), values.end(), [](StringValue a, StringValue b) {
		return a.value > b.value;
	});

	for each (auto value in values) {
		expenses.push_back({ value.string, wxNumberFormatter::ToString(value.value, 2) });
	}

	for each (auto budget in DataHelper::GetInstance().GetBudgets())
	{
		float currentAmount = 0.0;
		wxDateTime toDate = wxDateTime::Now();
		wxDateTime fromDate = wxDateTime::Now();

		if (budget->period == BudgetPeriods::Month) {
			fromDate.SetDay(1);
		}

		currentAmount = DataHelper::GetInstance().GetExpensesForBudget(budget.get(), &fromDate, &toDate);

		float remainAmount = budget->amount - currentAmount;
		float remainPercent = (currentAmount / budget->amount) * 100.0;

		budgets.push_back({ *budget->name, wxNumberFormatter::ToString(budget->amount, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(remainAmount, 2), remainPercent });
	}

	for each (auto goal in DataHelper::GetInstance().GetGoals()) {
		float currentAmount = DataHelper::GetInstance().GetBalanceForGoal(goal.get());

		float remainAmount = goal->amount - currentAmount;
		float remainPercent = currentAmount / (goal->amount / 100.0);
		
		goals.push_back({ *goal->name, wxNumberFormatter::ToString(goal->amount, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(remainAmount, 2), remainPercent });
	}

	totalExpenses = wxNumberFormatter::ToString(DataHelper::GetInstance().GetExpenses(&fromDate, &toDate), 2);

	Draw();
}

void DashboardPanel::Draw() {
	wxClientDC dc(this);
	
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	wxFont accountFont = this->GetFont();

	wxFont amountFont = this->GetFont();
	amountFont.SetWeight(wxFONTWEIGHT_BOLD);

	wxFont budgetFont = this->GetFont();
	budgetFont.SetPointSize(8);

	int progressWidth = 250;

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.SetFont(titleFont);

	int y = paddingY + 10;
	int x = paddingX + 10;
	int amountOffset = x + 250;

	dc.DrawText("Balance", wxPoint(x, y));
	
	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	wxSize size = dc.GetTextExtent(totalBalance);

	dc.DrawText(totalBalance, wxPoint(amountOffset - size.GetWidth(), y + 5));

	y = paddingY + 50;
	
	for each (auto account in balance) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account.name, wxPoint(x, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxSize size = dc.GetTextExtent(account.value);

		dc.DrawText(account.value, wxPoint(amountOffset - size.GetWidth(), y));

		y = y + 20;
	}

	if (virtualBalance.size() > 0) {
		y = y + 20;

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.SetFont(titleFont);

		dc.DrawText("Virtual balance", wxPoint(x, y));

		y = y + 40;

		for each (auto account in virtualBalance) {
			dc.SetFont(accountFont);
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(account.name, wxPoint(x, y));

			dc.SetFont(amountFont);
			dc.SetTextForeground(wxColor(60, 60, 60));

			wxSize size = dc.GetTextExtent(account.value);

			dc.DrawText(account.value, wxPoint(amountOffset - size.GetWidth(), y));

			y = y + 20;
		}
	}

	if (credits.size() > 0) {
		y = y + 20;

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.SetFont(titleFont);

		dc.DrawText("Credits", wxPoint(x, y));

		y = y + 40;

		for each (auto account in credits) {
			dc.SetFont(accountFont);
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(account.name, wxPoint(x, y));

			dc.SetFont(amountFont);
			dc.SetTextForeground(wxColor(60, 60, 60));

			wxSize size = dc.GetTextExtent(account.remainAmount);

			dc.DrawText(account.remainAmount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 23;

			dc.SetPen(wxPen(wxColor(200, 200, 200), 3));
			dc.DrawLine(x, y, x + progressWidth, y);

			int width = (progressWidth / 100.0) * account.percent;

			if (account.percent <= 50) {
				dc.SetPen(wxPen(color100, 3));
			}
			else if (account.percent > 50 && account.percent <= 80) {
				dc.SetPen(wxPen(color50, 3));
			}
			else {
				dc.SetPen(wxPen(color0, 3));
			}

			dc.DrawLine(x, y, x + width, y);

			y = y + 5;

			dc.SetFont(budgetFont);
			dc.SetTextForeground(wxColor(60, 60, 60));
			dc.DrawText(account.currentAmount, wxPoint(x, y));

			size = dc.GetTextExtent(account.amount);

			dc.SetFont(budgetFont);
			dc.SetTextForeground(wxColor(120, 120, 120));
			dc.DrawText(account.amount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 40;
		}
	}	

	dc.SetTextForeground(wxColor(0, 0, 0));	

	y = paddingY + 10;
	x = width / 2;

	amountOffset = x + 250;

	if (expenses.size() > 0) {
		dc.SetFont(titleFont);
		dc.DrawText("Expenses", wxPoint(x, y));
		
		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(120, 120, 120));

		wxSize size = dc.GetTextExtent(totalExpenses);

		dc.DrawText(totalExpenses, wxPoint(amountOffset - size.GetWidth(), y + 5));

		y = paddingY + 50;

		for each (auto account in expenses) {
			dc.SetFont(accountFont);
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(account.name, wxPoint(x, y));

			dc.SetFont(amountFont);
			dc.SetTextForeground(wxColor(60, 60, 60));

			wxSize size = dc.GetTextExtent(account.value);

			dc.DrawText(account.value, wxPoint(amountOffset - size.GetWidth(), y));

			y = y + 20;
		}
	}

	if (budgets.size() > 0) {
		y = y + 20;

		dc.SetFont(titleFont);
		dc.DrawText("Budgets", wxPoint(x, y));

		y = y + 40;

		for each (auto budget in budgets) {
			dc.SetFont(accountFont);
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(budget.name, wxPoint(x, y));

			dc.SetFont(amountFont);
			dc.SetTextForeground(wxColor(60, 60, 60));

			wxSize size = dc.GetTextExtent(budget.remainAmount);

			dc.DrawText(budget.remainAmount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 23;

			dc.SetPen(wxPen(wxColor(200, 200, 200), 3));
			dc.DrawLine(x, y, x + progressWidth, y);

			int width = (progressWidth / 100.0) * budget.percent;

			if (budget.percent <= 50) {
				dc.SetPen(wxPen(color0, 3));
			}
			else if (budget.percent > 50 && budget.percent <= 80) {
				dc.SetPen(wxPen(color50, 3));
			}
			else {
				dc.SetPen(wxPen(color100, 3));
			}

			if (width > progressWidth) {
				width = progressWidth;
			}

			dc.DrawLine(x, y, x + width, y);

			y = y + 5;

			dc.SetFont(budgetFont);

			if (budget.percent <= 100) {
				dc.SetTextForeground(wxColor(60, 60, 60));
			}
			else {
				dc.SetTextForeground(wxColor(185, 25, 0));
			}

			dc.DrawText(budget.currentAmount, wxPoint(x, y));

			size = dc.GetTextExtent(budget.amount);

			dc.SetFont(budgetFont);
			dc.SetTextForeground(wxColor(120, 120, 120));

			dc.DrawText(budget.amount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 40;
		}
	}

	if (goals.size() > 0) {
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.SetFont(titleFont);
		dc.DrawText("Goals", wxPoint(x, y));

		y = y + 40;

		for each (auto goal in goals) {
			dc.SetFont(accountFont);
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(goal.name, wxPoint(x, y));

			dc.SetFont(amountFont);
			dc.SetTextForeground(wxColor(60, 60, 60));

			wxSize size = dc.GetTextExtent(goal.remainAmount);

			dc.DrawText(goal.remainAmount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 23;

			dc.SetPen(wxPen(wxColor(200, 200, 200), 3));
			dc.DrawLine(x, y, x + progressWidth, y);

			int width = (progressWidth / 100.0) * goal.percent;

			if (goal.percent <= 50) {
				dc.SetPen(wxPen(color100, 3));
			}
			else if (goal.percent > 50 && goal.percent <= 80) {
				dc.SetPen(wxPen(color50, 3));
			}
			else {
				dc.SetPen(wxPen(color0, 3));
			}

			if (width > progressWidth) {
				width = progressWidth;
			}

			dc.DrawLine(x, y, x + width, y);

			y = y + 5;

			dc.SetFont(budgetFont);

			if (goal.percent <= 100) {
				dc.SetTextForeground(wxColor(60, 60, 60));
			}
			else {
				dc.SetTextForeground(wxColor(185, 25, 0));
			}

			dc.DrawText(goal.currentAmount, wxPoint(x, y));

			size = dc.GetTextExtent(goal.amount);

			dc.SetFont(budgetFont);
			dc.SetTextForeground(wxColor(120, 120, 120));

			dc.DrawText(goal.amount, wxPoint(x + progressWidth - size.GetWidth(), y));

			y = y + 40;
		}
	}
}

void DashboardPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}