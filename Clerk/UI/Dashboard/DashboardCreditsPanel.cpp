#include "DashboardCreditsPanel.h"

DashboardCreditsPanel::DashboardCreditsPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardCreditsPanel::OnPaint, this);
}

DashboardCreditsPanel::~DashboardCreditsPanel() {
}

void DashboardCreditsPanel::SetCredits(std::vector<std::shared_ptr<Account>> credits) {
	this->credits = credits;	
	values.clear();

	for (auto account : credits) {
		if (account->creditLimit > 0) {
			float amount = DataHelper::GetInstance().GetBalance(account.get());
			float currentAmount = account->creditLimit + amount;
			float remainPercent = abs(currentAmount / account->creditLimit) * 100.0;

			values.push_back({ *account->name, wxNumberFormatter::ToString(account->creditLimit, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(amount, 2), remainPercent });
		}
		else {
			float remainAmount = abs(DataHelper::GetInstance().GetBalance(account.get()));
			float amount = abs(DataHelper::GetInstance().GetAccountTotalExpense(account.get()));
			float currentAmount = DataHelper::GetInstance().GetAccountTotalReceipt(account.get());

			float remainPercent = (currentAmount / amount) * 100.0;

			values.push_back({ *account->name, wxNumberFormatter::ToString(amount, 2), wxNumberFormatter::ToString(currentAmount, 2),  wxNumberFormatter::ToString(remainAmount, 2), remainPercent });
		}
	}

	Update();
}

void DashboardCreditsPanel::Update()
{
	int height = 170 + (credits.size() * 30);
	this->SetMinSize(wxSize(-1, height));

	Draw();
}

void DashboardCreditsPanel::Draw() {
	wxClientDC dc(this);

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	dc.SetFont(titleFont);
	dc.DrawText("Credits", wxPoint(0, 0));

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

	int y = 60;

	dc.SetTextForeground(wxColor(151, 151, 151));
	dc.DrawText("0%", wxPoint(columnWidth0 + 20, y));
	dc.DrawText("100%", wxPoint(width - columnWidth1 - columnWidth2 - 65, y));

	wxSize size = dc.GetTextExtent("Credit");
	dc.DrawText("Credit", wxPoint(width - columnWidth2 - size.GetWidth() - 20, y));

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
		int percentWidth = (progressWidth / 100) * value.percent;

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
}

void DashboardCreditsPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}