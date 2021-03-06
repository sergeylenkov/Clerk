#include "Statusbar.h"

Statusbar::Statusbar(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxPanel(parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, "") {
	this->SetBackgroundColour(wxColour(245, 245, 245, 1));
	this->SetForegroundColour(wxColour(68, 68, 68, 1));
	this->SetMinSize(wxSize(-1, 20));

	wxBoxSizer *statusbarSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBitmap *bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_CALENDAR"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_periodLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	_periodLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	statusbarSizer->Add(_periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(20, 0, 0, wxEXPAND, 5);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_UP"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_receiptsLabel = new wxStaticText(this, wxID_ANY, wxT("0,00"), wxDefaultPosition, wxDefaultSize, 0);
	_receiptsLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_receiptsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_DOWN"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);
	
	_expensesLabel = new wxStaticText(this, wxID_ANY, wxT("0,00"), wxDefaultPosition, wxDefaultSize, 0);
	_expensesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_expensesLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	
	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_EXCHANGE_RATES"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_exchangeRatesLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	_exchangeRatesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_exchangeRatesLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_BALANCE"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_balanceLabel = new wxStaticText(this, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
	_balanceLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_balanceLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	this->SetSizer(statusbarSizer);
	this->Layout();
}

Statusbar::~Statusbar() {
	delete _viewModel;
}

void Statusbar::SetViewModel(StatusViewModel* viewModel) {
	_viewModel = viewModel;
	Update();
}

void Statusbar::Update() {
	_periodLabel->SetLabelText(wxDateTime::Now().Format("%B"));
	_receiptsLabel->SetLabelText(wxNumberFormatter::ToString(_viewModel->GetReceipts(), 2));
	_expensesLabel->SetLabelText(wxNumberFormatter::ToString(_viewModel->GetExpenses(), 2));
	_balanceLabel->SetLabelText(wxNumberFormatter::ToString(_viewModel->GetBalance(), 2));
	_exchangeRatesLabel->SetLabelText(_viewModel->GetExchangeRates());

	Layout();
}