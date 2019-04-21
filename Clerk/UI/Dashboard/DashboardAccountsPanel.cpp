#include "DashboardAccountsPanel.h"

DashboardAccountsPanel::DashboardAccountsPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardAccountsPanel::OnPaint, this);
}

DashboardAccountsPanel::~DashboardAccountsPanel() {
}

void DashboardAccountsPanel::SetAccounts(std::vector<StringValue> accounts) {
	this->accounts = accounts;	
	Update();
}

void DashboardAccountsPanel::Update()
{
	Draw();
}

void DashboardAccountsPanel::Draw() {
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
	dc.DrawText("Accounts", wxPoint(0, 0));

	wxFont accountFont = this->GetFont();
	wxFont amountFont = this->GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	int y = 50;

	for (auto account : accounts) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account.string, wxPoint(0, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxString value = wxNumberFormatter::ToString(account.value, 2);
		wxSize size = dc.GetTextExtent(value);

		dc.DrawText(value, wxPoint(width - size.GetWidth(), y));

		y = y + 30;
	}
}

void DashboardAccountsPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}