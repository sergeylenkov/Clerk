#include "DashboardAccountsPanel.h"

DashboardAccountsPanel::DashboardAccountsPanel(wxWindow *parent) : wxPanel(parent) {
	SetDoubleBuffered(true);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	_total = 0.0;
	_width = 0;
	_height = 0;

	Bind(wxEVT_PAINT, &DashboardAccountsPanel::OnPaint, this);
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

	int height = FromDIP(60 + (_viewModel->GetAccounts().size() * 24));
	SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardAccountsPanel::Draw(wxPaintDC &dc) {
	DoGetSize(&_width, &_height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont font = GetFont();

	wxFont titleFont = GetFont();
	titleFont.SetPointSize(12);

	wxFont hintFont = GetFont();
	hintFont.SetPointSize(8);

	dc.SetFont(titleFont);
	dc.DrawText(_("Accounts"), wxPoint(0, 0));	

	dc.SetFont(font);
	dc.SetTextForeground(wxColor(120, 120, 120));

	wxString value = Format::Amount(_total, _viewModel->GetCurrency()->sign);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(_width - size.GetWidth(), 5));

	int y = FromDIP(40);

	for (auto& account : _accounts) {
		dc.SetFont(font);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account->name, wxPoint(0, y));
		
		auto sign = account->currency->sign;

		if (account->isCredit) {
			if (account->balance == 0) {
				DrawValue(dc, Format::Amount(account->creditLimit, sign), y);
			} else {
				wxString value = Format::Amount(account->balance, sign);
				wxSize size = dc.GetTextExtent(value);				
				int x = _width - size.GetWidth();
				
				DrawValue(dc, value, y);

				float amount = account->creditLimit + account->balance;

				dc.SetFont(hintFont);

				value = Format::Amount(amount);
				size = dc.GetTextExtent(value);

				dc.SetTextForeground(wxColor(127, 127, 127));
				dc.DrawText(value, wxPoint(x - size.GetWidth() - FromDIP(10), y + FromDIP(2)));
			}			
		} else {
			if (Settings::GetInstance().IsShowBaseCurrencyAmount() && account->currency->id != _viewModel->GetCurrency()->id) {				
				wxString value = Format::Amount(account->balance, sign);
				wxSize size = dc.GetTextExtent(value);
				int x = _width - size.GetWidth();

				DrawValue(dc, value, y);

				float amount = _viewModel->ConvertAmountToBaseCurrency(*account->currency, account->balance);

				dc.SetFont(hintFont);

				value = Format::Amount(amount, _viewModel->GetCurrency()->sign);
				size = dc.GetTextExtent(value);

				dc.SetTextForeground(wxColor(127, 127, 127));
				dc.DrawText(value, wxPoint(x - size.GetWidth() - FromDIP(10), y + FromDIP(2)));
			}
			else {
				DrawValue(dc, Format::Amount(account->balance, sign), y);
			}
		}

		y = y + FromDIP(24);
	}
}

void DashboardAccountsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}

void DashboardAccountsPanel::DrawValue(wxPaintDC& dc, wxString value, int y) {
	wxSize size = dc.GetTextExtent(value);

	dc.SetTextForeground(wxColor(60, 60, 60));
	dc.DrawText(value, wxPoint(_width - size.GetWidth(), y));
}