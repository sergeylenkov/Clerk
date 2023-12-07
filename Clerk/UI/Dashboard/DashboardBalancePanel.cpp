#include "DashboardBalancePanel.h"

DashboardBalancePanel::DashboardBalancePanel(wxWindow *parent) : wxPanel(parent) {
	SetDoubleBuffered(true);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	Bind(wxEVT_PAINT, &DashboardBalancePanel::OnPaint, this);
}

DashboardBalancePanel::~DashboardBalancePanel() {
}

void DashboardBalancePanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([=]() {
		Update();
	});

	Update();
}

void DashboardBalancePanel::Update()
{
	_total = _viewModel->GetTotalFunds();
	_ownFunds = _viewModel->GetOwnFunds();
	_creditFunds = _viewModel->GetCreditFunds();

	SetMinSize(FromDIP(wxSize(-1, 130)));
	Refresh();
}

void DashboardBalancePanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText(_("Balance"), wxPoint(0, 0));

	wxFont balanceFont = GetFont();
	balanceFont.SetPointSize(12);

	wxFont font = GetFont();
	font.SetPointSize(10);

	int columnWidth = 0;
	int y = FromDIP(30);

	dc.SetFont(balanceFont);
	
	wxString value = Format::Amount(_total, _viewModel->GetCurrency()->sign);
	dc.DrawText(value, wxPoint(0, y));

	y = FromDIP(60);

	dc.SetFont(font);

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText(_("Own funds"), wxPoint(0, y));
	
	y = y + FromDIP(25);

	value = Format::Amount(_ownFunds, _viewModel->GetCurrency()->sign);
	wxSize size = dc.GetTextExtent(value);
	
	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.DrawText(value, wxPoint(0, y));
	
	if (size.GetWidth() > columnWidth) {
		columnWidth = size.GetWidth();
	}

	y = FromDIP(60);
	int x = columnWidth + FromDIP(80);

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText(_("Credit funds"), wxPoint(x, y));

	y = y + FromDIP(25);

	value = Format::Amount(_creditFunds, _viewModel->GetCurrency()->sign);
	size = dc.GetTextExtent(value);

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.DrawText(value, wxPoint(x, y));
}

void DashboardBalancePanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}