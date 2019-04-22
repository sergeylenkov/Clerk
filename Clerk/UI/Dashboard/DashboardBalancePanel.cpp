#include "DashboardBalancePanel.h"

DashboardBalancePanel::DashboardBalancePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardBalancePanel::OnPaint, this);
}

DashboardBalancePanel::~DashboardBalancePanel() {
}

void DashboardBalancePanel::SetBalance(std::map<wxString, float> own, std::map<wxString, float> credit) {
	ownFunds = own;
	creditFunds = credit;

	Update();
}

void DashboardBalancePanel::Update()
{
	Draw();
}

void DashboardBalancePanel::Draw() {
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
	dc.DrawText("Balance", wxPoint(0, 0));

	wxFont font = this->GetFont();
	font.SetPointSize(10);

	int columnWidth = 0;
	int y = 50;

	dc.SetFont(font);

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Own funds", wxPoint(0, y));

	y = y + 40;

	for (auto &fund : ownFunds) {
		wxString value = wxNumberFormatter::ToString(fund.second, 2);
		wxSize size = dc.GetTextExtent(value);

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(value, wxPoint(0, y));
		dc.DrawText(fund.first, wxPoint(size.GetWidth() + 5, y));		

		if (size.GetWidth() > columnWidth) {
			columnWidth = size.GetWidth();
		}

		y = y + 30;
	}

	y = 50;
	int x = columnWidth + 60;

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Credit funds", wxPoint(x, y));

	y = y + 40;	

	for (auto &fund : creditFunds) {
		wxString value = wxNumberFormatter::ToString(fund.second, 2);
		wxSize size = dc.GetTextExtent(value);

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(value, wxPoint(x, y));		
		dc.DrawText(fund.first, wxPoint(x + size.GetWidth() + 5, y));

		y = y + 30;
	}
}

void DashboardBalancePanel::OnPaint(wxPaintEvent& event) {
	Draw();
}