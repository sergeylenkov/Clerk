#include "DashboardExpensesPanel.h"

DashboardExpensesPanel::DashboardExpensesPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardExpensesPanel::OnPaint, this);
}

void DashboardExpensesPanel::SetExpenses(std::vector<StringValue> expenses) {
	this->expenses = expenses;

	totalValue = 0;
	maxValue = 0;

	std::sort(this->expenses.begin(), this->expenses.end(), [](StringValue a, StringValue b) {
		return a.value > b.value;
	});

	for (auto expense : expenses) {
		totalValue = totalValue + expense.value;

		if (expense.value > maxValue) {
			maxValue = expense.value;
		}
	}	

	Update();
}

void DashboardExpensesPanel::Update()
{
	int height = 50 + (expenses.size() * 30);
	this->SetMinSize(wxSize(-1, height));

	Draw();
}

void DashboardExpensesPanel::Draw() {
	wxClientDC dc(this);

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);	

	dc.SetFont(titleFont);
	dc.DrawText("Expenses", wxPoint(0, 0));

	wxFont accountFont = this->GetFont();
	wxFont amountFont = this->GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	wxString value = wxNumberFormatter::ToString(totalValue, 2);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	int y = 50;
	
	for (auto expense : expenses) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(expense.string, wxPoint(0, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxString value = wxNumberFormatter::ToString(expense.value, 2);
		wxSize size = dc.GetTextExtent(value);

		dc.DrawText(value, wxPoint(width - size.GetWidth(), y));

		int percent = (expense.value / maxValue) * 100;
		int progressWidth = (width / 100.0) * percent;

		if (progressWidth < 5) {
			progressWidth = 5;
		}

		dc.SetPen(wxPen(wxColor(10, 199, 117), 1));
		dc.SetBrush(wxBrush(wxColor(10, 199, 117)));

		dc.DrawRectangle(0, y + 20, progressWidth, 2);

		y = y + 30;
	}
}

void DashboardExpensesPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}