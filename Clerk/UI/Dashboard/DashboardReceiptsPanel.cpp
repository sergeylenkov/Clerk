#include "DashboardReceiptsPanel.h"

DashboardReceiptsPanel::DashboardReceiptsPanel(wxWindow* parent) : wxPanel(parent) {
	SetDoubleBuffered(true);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	Bind(wxEVT_PAINT, &DashboardReceiptsPanel::OnPaint, this);
}

void DashboardReceiptsPanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([=]() {
		Update();
		});

	Update();
}

void DashboardReceiptsPanel::Update()
{
	_accounts = _viewModel->GetReceiptsForMonth();

	if (_accounts.empty()) {
		SetMinSize(wxSize(-1, -1));
		return;
	}

	_total = _viewModel->GetTotalReceiptsForMonth();

	_maxValue = 0;

	for (auto& account : _accounts) {
		if (account->receipts > _maxValue) {
			_maxValue = account->receipts;
		}
	}

	int height = FromDIP(60 + (_accounts.size() * 30));
	SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardReceiptsPanel::Draw(wxPaintDC& dc) {
	int width = 0;
	int height = 0;

	DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText(_("Receipts"), wxPoint(0, 0));

	wxFont accountFont = GetFont();
	wxFont amountFont = GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	wxString value = Format::Amount(_total, _viewModel->GetCurrency()->sign);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	int y = FromDIP(40);

	for (auto& account : _accounts) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account->name, wxPoint(0, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxString value = Format::Amount(account->receipts, account->currency->sign);
		wxSize size = dc.GetTextExtent(value);

		dc.DrawText(value, wxPoint(width - size.GetWidth(), y));

		int percent = (account->receipts / _maxValue) * 100;
		int progressWidth = (width / 100.0) * percent;

		if (progressWidth < 5) {
			progressWidth = 5;
		}

		dc.SetPen(wxPen(wxColor(10, 199, 117), 1));
		dc.SetBrush(wxBrush(wxColor(10, 199, 117)));

		dc.DrawRectangle(0, y + FromDIP(20), progressWidth, 2);

		y = y + FromDIP(30);
	}
}

void DashboardReceiptsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}