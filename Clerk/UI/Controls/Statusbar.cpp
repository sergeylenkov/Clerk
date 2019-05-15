#include "Statusbar.h"

Statusbar::Statusbar(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxPanel(parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, "") {
	this->SetBackgroundColour(wxColour(245, 245, 245, 1));
	this->SetForegroundColour(wxColour(68, 68, 68, 1));
	this->SetMinSize(wxSize(-1, 20));

	wxBoxSizer *statusbarSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBitmap *bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_CALENDAR"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	periodLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	periodLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	statusbarSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(20, 0, 0, wxEXPAND, 5);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_UP"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	receiptsLabel = new wxStaticText(this, wxID_ANY, wxT("0,00"), wxDefaultPosition, wxDefaultSize, 0);
	receiptsLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(receiptsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_DOWN"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);
	
	expensesLabel = new wxStaticText(this, wxID_ANY, wxT("0,00"), wxDefaultPosition, wxDefaultSize, 0);
	expensesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(expensesLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	
	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_EXCHANGE_RATES"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	exchangeRatesLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	exchangeRatesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(exchangeRatesLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ICON_STATUSBAR_BALANCE"), wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxDefaultSize, 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	balanceLabel = new wxStaticText(this, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
	balanceLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(balanceLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	this->SetSizer(statusbarSizer);
	this->Layout();
}

void Statusbar::SetPeriod(wxString value) {
	periodLabel->SetLabelText(value);
}

void Statusbar::SetRecepipts(wxString value) {
	receiptsLabel->SetLabelText(value);
}

void Statusbar::SetExpenses(wxString value) {
	expensesLabel->SetLabelText(value);
}

void Statusbar::SetBalance(wxString value) {
	balanceLabel->SetLabelText(value);
}

void Statusbar::SetExchangeRates(wxString value) {
	exchangeRatesLabel->SetLabelText(value);
}