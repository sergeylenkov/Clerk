#include "DashboardExpensesPanel.h"

DashboardExpensesPanel::DashboardExpensesPanel(wxWindow *parent) : wxPanel(parent) {
	maxValue = 0;
	this->Bind(wxEVT_PAINT, &DashboardExpensesPanel::OnPaint, this);
}

void DashboardExpensesPanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [=]() {
		Update();
	};

	Update();
}

void DashboardExpensesPanel::Update()
{
	auto expenses = _viewModel->GetExpensesForMonth();

	maxValue = 0;

	for (auto& account : expenses) {
		if (account->balance > maxValue) {
			maxValue = account->balance;
		}
	}

	int height = 50 + (expenses.size() * 35);
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

	wxString value = Format::Amount(_viewModel->GetTotalExpensesForMonth(), *_viewModel->GetCurrency().sign);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	int y = 50;
	
	for (auto &account : _viewModel->GetExpensesForMonth()) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account->name, wxPoint(0, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxString value = Format::Amount(account->balance, *account->currency->sign);
		wxSize size = dc.GetTextExtent(value);

		dc.DrawText(value, wxPoint(width - size.GetWidth(), y));

		int percent = (account->balance / maxValue) * 100;
		int progressWidth = (width / 100.0) * percent;

		if (progressWidth < 5) {
			progressWidth = 5;
		}

		dc.SetPen(wxPen(wxColor(10, 199, 117), 1));
		dc.SetBrush(wxBrush(wxColor(10, 199, 117)));

		dc.DrawRectangle(0, y + 20, progressWidth, 2);

		y = y + 35;
	}
}

void DashboardExpensesPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}