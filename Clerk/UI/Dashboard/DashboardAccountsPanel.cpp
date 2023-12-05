#include "DashboardAccountsPanel.h"

DashboardAccountsPanel::DashboardAccountsPanel(wxWindow *parent) : wxPanel(parent) {
	this->SetDoubleBuffered(true);
	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	this->Bind(wxEVT_PAINT, &DashboardAccountsPanel::OnPaint, this);
}

void DashboardAccountsPanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([=]() {
		Update();
	});

	Update();
}

void DashboardAccountsPanel::Update()
{
	_accounts = _viewModel->GetAccounts();
	_total = _viewModel->GetOwnFunds();

	int height = this->FromDIP(60 + (_viewModel->GetAccounts().size() * 24));
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

	wxString value = Format::Amount(_total, _viewModel->GetCurrency()->sign);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	int y = this->FromDIP(40);

	for (auto& account : _accounts) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account->name, wxPoint(0, y));

		dc.SetFont(amountFont);
		
		auto sign = account->currency->sign;

		if (account->isCredit) {
			if (account->balance == 0) {
				wxString value = Format::Amount(account->creditLimit, sign);
				wxSize size = dc.GetTextExtent(value);
		
				dc.SetTextForeground(wxColor(60, 60, 60));
				dc.DrawText(value, wxPoint(width - size.GetWidth(), y));
			} else {
				float amount = account->creditLimit + account->balance;

				wxString value = Format::Amount(account->balance, sign);
				wxSize size = dc.GetTextExtent(value);				
				int x = width - size.GetWidth();
				
				dc.SetTextForeground(wxColor(60, 60, 60));
				dc.DrawText(value, wxPoint(x, y));

				value = Format::Amount(amount);
				size = dc.GetTextExtent(value);

				dc.SetTextForeground(wxColor(127, 127, 127));
				dc.DrawText(value, wxPoint(x - size.GetWidth() - 10, y));
			}
			
		} else {
			wxString value = Format::Amount(account->balance, sign);
			wxSize size = dc.GetTextExtent(value);

			dc.SetTextForeground(wxColor(60, 60, 60));
			dc.DrawText(value, wxPoint(width - size.GetWidth(), y));
		}

		y = y + this->FromDIP(24);
	}
}

void DashboardAccountsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}