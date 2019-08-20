#include "DashboardAccountsPanel.h"

DashboardAccountsPanel::DashboardAccountsPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardAccountsPanel::OnPaint, this);
}

void DashboardAccountsPanel::SetAccounts(std::vector<AccountValue> accounts) {
	this->accounts = accounts;	
	Update();
}

void DashboardAccountsPanel::Update()
{
	int height = 70 + (accounts.size() * 30);
	this->SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardAccountsPanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText("Accounts", wxPoint(0, 0));

	wxFont accountFont = this->GetFont();
	wxFont amountFont = this->GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	int y = 50;

	for (auto &item : accounts) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(*item.account->name, wxPoint(0, y));

		dc.SetFont(amountFont);

		if (item.account->isCredit) {
			if (item.value == 0) {
				wxString value = Utils::FormatAmount(item.account->creditLimit, item.account->currency.get());
				wxSize size = dc.GetTextExtent(value);
		
				dc.SetTextForeground(wxColor(60, 60, 60));
				dc.DrawText(value, wxPoint(width - size.GetWidth(), y));
			}
			else {
				float amount = item.account->creditLimit + item.account->balance;

				wxString value = Utils::FormatAmount(item.value, item.account->currency.get());
				wxSize size = dc.GetTextExtent(value);				
				int x = width - size.GetWidth();
				
				dc.SetTextForeground(wxColor(60, 60, 60));
				dc.DrawText(value, wxPoint(x, y));

				value = Utils::FormatAmount(amount);
				size = dc.GetTextExtent(value);

				dc.SetTextForeground(wxColor(127, 127, 127));
				dc.DrawText(value, wxPoint(x - size.GetWidth() - 10, y));
			}
			
		}
		else {
			wxString value = Utils::FormatAmount(item.value, item.account->currency.get());
			wxSize size = dc.GetTextExtent(value);

			dc.SetTextForeground(wxColor(60, 60, 60));
			dc.DrawText(value, wxPoint(width - size.GetWidth(), y));
		}

		y = y + 30;
	}
}

void DashboardAccountsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}