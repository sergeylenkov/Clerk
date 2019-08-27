#include "DashboardBalancePanel.h"

DashboardBalancePanel::DashboardBalancePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardBalancePanel::OnPaint, this);
}

void DashboardBalancePanel::SetBalance(CurrencyValue balance, std::vector<CurrencyValue> own, std::vector<CurrencyValue> credit) {
	ownFunds = own;
	creditFunds = credit;
	totalBalance = balance;

	Update();
}

void DashboardBalancePanel::Update()
{
	int height = 140 + (ownFunds.size() * 30);	
	this->SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardBalancePanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText("Balance", wxPoint(0, 0));

	wxFont balanceFont = this->GetFont();
	balanceFont.SetPointSize(12);

	wxFont font = this->GetFont();
	font.SetPointSize(10);

	int columnWidth = 0;
	int y = 40;

	dc.SetFont(balanceFont);

	wxString value = Utils::FormatAmount(totalBalance.value, totalBalance.currency);
	dc.DrawText(value, wxPoint(0, y));

	y = 80;

	dc.SetFont(font);

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Own funds", wxPoint(0, y));

	y = y + 40;

	for (auto &fund : ownFunds) {
		wxString value = Utils::FormatAmount(fund.value, fund.currency);
		wxSize size = dc.GetTextExtent(value);

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(value, wxPoint(0, y));

		if (size.GetWidth() > columnWidth) {
			columnWidth = size.GetWidth();
		}

		y = y + 30;
	}

	y = 80;
	int x = columnWidth + 80;

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Credit funds", wxPoint(x, y));

	y = y + 40;	

	for (auto &fund : creditFunds) {
		wxString value = Utils::FormatAmount(fund.value, fund.currency);
		wxSize size = dc.GetTextExtent(value);

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(value, wxPoint(x, y));

		y = y + 30;
	}
}

void DashboardBalancePanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}