#include "Statusbar.h"

Statusbar::Statusbar(StatusViewModel& viewModel, wxWindow *parent, const wxPoint &pos, const wxSize &size):
	wxPanel(parent, wxID_ANY, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, ""),
	_viewModel(viewModel) {
	SetBackgroundColour(wxColour(245, 245, 245, 1));
	SetForegroundColour(wxColour(68, 68, 68, 1));
	SetMinSize(size);

	wxBoxSizer *statusbarSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBitmap *bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap("ICON_STATUSBAR_CALENDAR", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, this->FromDIP(wxSize(16, 16)), 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_periodLabel = new wxStaticText(this, wxID_ANY, "");
	_periodLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	statusbarSizer->Add(_periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(20, 0, 0, wxEXPAND, 5);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap("ICON_STATUSBAR_DOWN", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, this->FromDIP(wxSize(16, 16)), 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_receiptsLabel = new wxStaticText(this, wxID_ANY, "0,00");
	_receiptsLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_receiptsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap("ICON_STATUSBAR_UP", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, this->FromDIP(wxSize(16, 16)), 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);
	
	_expensesLabel = new wxStaticText(this, wxID_ANY, "0,00");
	_expensesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_expensesLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	
	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);
	
	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap("ICON_STATUSBAR_BALANCE", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, this->FromDIP(wxSize(16, 16)), 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_balanceLabel = new wxStaticText(this, wxID_ANY, "0.00");
	_balanceLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_balanceLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	statusbarSizer->Add(0, 0, 1, wxEXPAND, 0);

	bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap("ICON_STATUSBAR_EXCHANGE_RATES", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, this->FromDIP(wxSize(18, 18)), 0);
	statusbarSizer->Add(bitmap, 0, wxALL, 5);

	_exchangeRatesLabel = new wxStaticText(this, wxID_ANY, "");
	_exchangeRatesLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	statusbarSizer->Add(_exchangeRatesLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	
	_viewModel.OnUpdate([&]() {
		Update();
	});

	Update();

	SetSizer(statusbarSizer);
	Layout();
}

Statusbar::~Statusbar() {
	delete& _viewModel;
}

void Statusbar::Update() {
	wxString sign = _viewModel.GetBaseCurrency()->sign;
	wxString receiptsValue = Format::Amount(_viewModel.GetReceipts(), sign);
	wxString expensesValue = Format::Amount(_viewModel.GetExpenses(), sign);
	wxString balanceValue = Format::Amount(_viewModel.GetBalance(), sign);

	_periodLabel->SetLabelText(wxDateTime::Now().Format("%B"));
	_receiptsLabel->SetLabelText(receiptsValue);
	_expensesLabel->SetLabelText(expensesValue);
	_balanceLabel->SetLabelText(balanceValue);

	if (_viewModel.GetIsExchangeRatesLoading()) {
		_exchangeRatesLabel->SetLabelText(_("Updating..."));
	}
	else {
		_exchangeRatesLabel->SetLabelText(_viewModel.GetExchangeRates());
	}

	Layout();
}