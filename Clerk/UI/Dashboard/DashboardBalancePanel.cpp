#include "DashboardBalancePanel.h"

DashboardBalancePanel::DashboardBalancePanel(wxWindow *parent) : wxPanel(parent) {
	this->Bind(wxEVT_PAINT, &DashboardBalancePanel::OnPaint, this);
}

DashboardBalancePanel::~DashboardBalancePanel() {
	delete _viewModel;
}

void DashboardBalancePanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [=]() {
		Update();
	};

	Update();
}

void DashboardBalancePanel::Update()
{
	SetMinSize(wxSize(-1, 170));
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

	wxString value = Format::Amount(_viewModel->GetTotalFunds(), *_viewModel->GetCurrency().sign);
	dc.DrawText(value, wxPoint(0, y));

	y = 80;

	dc.SetFont(font);

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Own funds", wxPoint(0, y));

	y = y + 40;

	value = Format::Amount(_viewModel->GetOwnFunds(), *_viewModel->GetCurrency().sign);
	wxSize size = dc.GetTextExtent(value);

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.DrawText(value, wxPoint(0, y));

	if (size.GetWidth() > columnWidth) {
		columnWidth = size.GetWidth();
	}

	y = 80;
	int x = columnWidth + 80;

	dc.SetTextForeground(wxColor(127, 127, 127));
	dc.DrawText("Credit funds", wxPoint(x, y));

	y = y + 40;	

	value = Format::Amount(_viewModel->GetCreditFunds(), *_viewModel->GetCurrency().sign);
	size = dc.GetTextExtent(value);

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.DrawText(value, wxPoint(x, y));
}

void DashboardBalancePanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}