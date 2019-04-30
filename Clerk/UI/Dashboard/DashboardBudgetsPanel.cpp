#include "DashboardBudgetsPanel.h"

DashboardBudgetsPanel::DashboardBudgetsPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardBudgetsPanel::OnPaint, this);
}

void DashboardBudgetsPanel::SetBudgets(std::vector<std::shared_ptr<Budget>> budgets) {
	this->budgets = budgets;
	values.clear();

	wxDateTime toDate = wxDateTime::Now();
	wxDateTime fromDate = wxDateTime::Now();

	for (auto budget : budgets) {
		if (budget->period == BudgetPeriods::Month) {
			fromDate.SetDay(1);
		}

		float currentAmount = DataHelper::GetInstance().GetExpensesForBudget(budget.get(), &fromDate, &toDate);
		float remainAmount = budget->amount - currentAmount;
		float remainPercent = (currentAmount / budget->amount) * 100.0;

		values.push_back({ *budget->name, wxNumberFormatter::ToString(budget->amount, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(remainAmount, 2), remainPercent });
	}

	Update();
}

void DashboardBudgetsPanel::Update()
{
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	daysCount = toDate.GetDay();
	currentDay = wxDateTime::Now().GetDay();

	int height = 170 + (budgets.size() * 40);
	this->SetMinSize(wxSize(-1, height));

	Draw();
}

void DashboardBudgetsPanel::Draw() {	
	wxClientDC dc(this);

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);
	//titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	dc.SetFont(titleFont);
	dc.DrawText("Budgets", wxPoint(0, 0));

	wxFont font = this->GetFont();
	font.SetPointSize(8);

	dc.SetFont(font);

	int columnWidth0 = 0;
	int columnWidth1 = 0;
	int columnWidth2 = 0;
	
	for (auto value : values) {
		wxSize size = dc.GetTextExtent(value.name);

		if (size.GetWidth() > columnWidth0) {
			columnWidth0 = size.GetWidth();
		}

		size = dc.GetTextExtent(value.amount);

		if (size.GetWidth() > columnWidth1) {
			columnWidth1 = size.GetWidth();
		}

		size = dc.GetTextExtent(value.remainAmount);

		if (size.GetWidth() > columnWidth2) {
			columnWidth2 = size.GetWidth();
		}
	}

	int y = 40;
	
	dc.SetTextForeground(wxColor(151, 151, 151));
	dc.DrawText("0%", wxPoint(columnWidth0 + 20, y));
	dc.DrawText("100%", wxPoint(width - columnWidth1 - columnWidth2 - 65, y));

	wxSize size = dc.GetTextExtent("Budget");
	dc.DrawText("Budget", wxPoint(width - columnWidth2 - size.GetWidth() - 20, y));

	size = dc.GetTextExtent("Remain");
	dc.DrawText("Remain", wxPoint(width - size.GetWidth(), y));

	y = y + 20;

	dc.SetPen(wxPen(wxColor(203, 203, 203), 1));
	dc.DrawLine(0, y, width, y);

	int x = 0;
	y = y + 40;

	int progressWidth = width - columnWidth0 - columnWidth1 - columnWidth2 - 60;
	int progressX = columnWidth0 + 20;

	for (auto value : values) {
		dc.SetTextForeground(wxColor(0, 0, 0));

		dc.SetFont(font);
		dc.DrawText(value.name, wxPoint(0, y));

		wxSize size = dc.GetTextExtent(value.amount);
		x = width - columnWidth2 - size.GetWidth() - 20;

		dc.DrawText(value.amount, wxPoint(x, y));

		size = dc.GetTextExtent(value.remainAmount);
		dc.DrawText(value.remainAmount, wxPoint(width - size.GetWidth(), y));

		int progressY = y + size.GetHeight() / 2;
		int percentWidth = (progressWidth / 100.0) * value.percent;

		if (percentWidth > progressWidth) {
			percentWidth = progressWidth;
		}

		dc.SetPen(wxPen(wxColor(216, 216, 216), 1));
		dc.SetBrush(wxBrush(wxColor(216, 216, 216)));

		dc.DrawRectangle(progressX, progressY, progressWidth, 4);

		dc.SetPen(wxPen(wxColor(10, 199, 117), 1));
		dc.SetBrush(wxBrush(wxColor(10, 199, 117)));

		if (value.percent > 90) {
			dc.SetPen(wxPen(wxColor(242, 73, 101), 1));
			dc.SetBrush(wxBrush(wxColor(242, 73, 101)));
		}

		dc.DrawRectangle(progressX, progressY, percentWidth, 4);

		size = dc.GetTextExtent(value.currentAmount);

		int amountX = percentWidth - (size.GetWidth() / 2);

		if (amountX < 0) {
			amountX = 0;
		}
		else if (amountX + size.GetWidth() > progressWidth) {
			amountX = progressWidth - size.GetWidth();
		}

		dc.DrawText(value.currentAmount, wxPoint(progressX + amountX, y + 14));

		y = y + 40;
	}

	int dayPercent = (currentDay / (float)daysCount) * 100;
	x = (progressWidth / 100.0) * dayPercent;

	dc.SetPen(wxPen(wxColor(204, 204, 204), 1));
	dc.DrawLine(progressX + x, 100, progressX + x, y);

	wxString monthStr = wxDateTime::Now().Format("%b %d").Upper();
	size = dc.GetTextExtent(monthStr);

	int rectWidth = size.GetWidth() + 10;
	int rectHeight = size.GetHeight() + 5;

	dc.SetPen(wxPen(wxColor(153, 153, 153), 1));
	dc.SetBrush(wxBrush(wxColor(153, 153, 153)));
	dc.DrawRectangle((progressX + x) - (rectWidth / 2), y, rectWidth, rectHeight);
	
	dc.SetTextForeground(wxColor(255, 255, 255));
	dc.DrawText(monthStr, wxPoint((progressX + x) - (size.GetWidth() / 2), y + 2));
}

void DashboardBudgetsPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}