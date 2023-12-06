#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, DataContext& context) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)),
	_context(context) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));

	int indent = this->FromDIP(5);
	int bottomIndent = this->FromDIP(15);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* currencyLabel = new wxStaticText(this, wxID_ANY, _("Base Currency:"));
	horizontalSizer->Add(currencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_currencyList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_currencyList, 1, wxALIGN_CENTER_VERTICAL);

	verticalSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_convertCurrenciesCheckBox = new wxCheckBox(this, wxID_ANY, _("Convert currencies by exchange rate"));
	horizontalSizer->Add(_convertCurrenciesCheckBox);

	verticalSizer->Add(horizontalSizer, 0, wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_loadExchangeRatesCheckBox = new wxCheckBox(this, wxID_ANY, _("Load exchange rates on start"));
	horizontalSizer->Add(_loadExchangeRatesCheckBox);

	verticalSizer->Add(horizontalSizer, 0, wxBOTTOM, bottomIndent);

	mainSizer->Add(verticalSizer, 1, wxEXPAND | wxALL, indent * 2);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* okButton = new wxButton(this, wxID_ANY, _("OK"));
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	wxButton* cancelButton = new wxButton(this, wxID_ANY, _("Cancel"));
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, indent * 2);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
		
	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	int index = 0;
	int i = 0;
	
	for (auto currency : _context.GetCurrenciesService().GetAll())
	{
		_currencies.push_back(currency);

		wxString name = wxString::Format("%s (%s)", currency->shortName, currency->name);
		_currencyList->AppendString(name);

		if (baseCurrencyId == currency->id) {
			index = i;
		}

		i++;
	}

	_currencyList->SetSelection(index);

	_convertCurrenciesCheckBox->SetValue(Settings::GetInstance().IsConvertCurrency());
	_loadExchangeRatesCheckBox->SetValue(Settings::GetInstance().IsLoadExchangeRates());

	okButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &PreferencesDialog::OnKeyDown, this);
}

void PreferencesDialog::OnOK(wxCommandEvent &event) {
	auto currency = _currencies[_currencyList->GetSelection()];
	Settings::GetInstance().SetBaseCurrencyId(currency->id);

	Settings::GetInstance().SetConvertCurrency(_convertCurrenciesCheckBox->IsChecked());
	Settings::GetInstance().SetLoadExchangeRates(_loadExchangeRatesCheckBox->IsChecked());

	Close();
}

void PreferencesDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void PreferencesDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}