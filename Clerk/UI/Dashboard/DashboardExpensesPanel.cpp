#include "DashboardExpensesPanel.h"

DashboardExpensesPanel::DashboardExpensesPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardExpensesPanel::OnPaint, this);
}

void DashboardExpensesPanel::SetExpenses(std::vector<AccountValue> expenses) {
	this->expenses = expenses;
	
	maxValue = 0;

	if (expenses.size() > 0) {
		std::sort(this->expenses.begin(), this->expenses.end(), [](AccountValue a, AccountValue b) {
			return a.value > b.value;
		});

		totalValue = { *expenses[0].account->currency.get(), 0 };

		for (auto &expense : expenses) {
			totalValue.value = totalValue.value + expense.value;

			if (expense.value > maxValue) {
				maxValue = expense.value;
			}
		}
	}		

	Update();
}

void DashboardExpensesPanel::Update()
{
	int height = 50 + (expenses.size() * 30);
	this->SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardExpensesPanel::Draw(wxPaintDC &dc) {
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

	wxString value = Utils::FormatAmount(totalValue.value, totalValue.currency);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	int y = 50;
	
	for (auto &item : expenses) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(*item.account->name, wxPoint(0, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxString value = Utils::FormatAmount(item.value, *item.account->currency);
		wxSize size = dc.GetTextExtent(value);

		dc.DrawText(value, wxPoint(width - size.GetWidth(), y));

		int percent = (item.value / maxValue) * 100;
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
	wxPaintDC dc(this);
	Draw(dc);
}