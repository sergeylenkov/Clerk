#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, DataContext& context) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)),
	_context(context) {
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("General")), wxVERTICAL);

	_showCreditFundsCheckBox = new wxCheckBox(this, wxID_ANY, _("Show credit funds in balance"));
	staticBoxSizer->Add(_showCreditFundsCheckBox, 0, wxALL, indent);

	verticalSizer->Add(staticBoxSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Language")), wxVERTICAL);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* languageLabel = new wxStaticText(this, wxID_ANY, _("Language:"));
	horizontalSizer->Add(languageLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_languagesList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(100, -1)), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_languagesList, 1, wxALIGN_CENTER_VERTICAL);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL, indent);

	verticalSizer->Add(staticBoxSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Currencies")), wxVERTICAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* currencyLabel = new wxStaticText(this, wxID_ANY, _("Base Currency:"));
	horizontalSizer->Add(currencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_currenciesList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_currenciesList, 1, wxALIGN_CENTER_VERTICAL);

	staticBoxSizer->Add(horizontalSizer, 0, wxEXPAND | wxALL, indent);

	_loadExchangeRatesCheckBox = new wxCheckBox(this, wxID_ANY, _("Load exchange rates on start"));

	staticBoxSizer->Add(_loadExchangeRatesCheckBox, 0, wxALL, indent);

	_convertCurrenciesCheckBox = new wxCheckBox(this, wxID_ANY, _("Convert amount by exchange rate"));

	staticBoxSizer->Add(_convertCurrenciesCheckBox, 0, wxALL, indent);

	_showBaseCurrencyAmountCheckBox = new wxCheckBox(this, wxID_ANY, _("Show amount in base currency"));

	staticBoxSizer->Add(_showBaseCurrencyAmountCheckBox, 0, wxALL, indent);

	verticalSizer->Add(staticBoxSizer, 0, wxEXPAND);

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
	
	_currencies = _context.GetCurrenciesService().GetAll();
	_languages = Settings::GetInstance().GetLanguages();

	int currentLanguage = Settings::GetInstance().GetLanguage();	

	int index = 0;
	bool found = false;

	for (auto language : _languages)
	{
		_languagesList->AppendString(language.name);

		if (language.id == currentLanguage) {
			found = true;
		}
		else if (language.id != currentLanguage && !found) {
			index++;
		}
	}

	_languagesList->SetSelection(index);

	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	index = 0;
	found = false;

	for (auto currency : _currencies)
	{
		wxString name = wxString::Format("%s (%s)", currency->shortName, currency->name);
		_currenciesList->AppendString(name);

		if (currency->id == baseCurrencyId) {
			found = true;
		} else if (currency->id != baseCurrencyId && !found) {
			index++;
		}
	}

	_currenciesList->SetSelection(index);

	_convertCurrenciesCheckBox->SetValue(Settings::GetInstance().IsConvertCurrency());
	_loadExchangeRatesCheckBox->SetValue(Settings::GetInstance().IsLoadExchangeRates());
	_showCreditFundsCheckBox ->SetValue(Settings::GetInstance().IsShowCreditFunds());
	_showBaseCurrencyAmountCheckBox->SetValue(Settings::GetInstance().IsShowBaseCurrencyAmount());

	okButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &PreferencesDialog::OnKeyDown, this);
}

void PreferencesDialog::OnOK(wxCommandEvent &event) {
	auto language = _languages[_languagesList->GetSelection()];

	Settings::GetInstance().SetLanguage(language.id);

	auto currency = _currencies[_currenciesList->GetSelection()];

	Settings::GetInstance().SetBaseCurrencyId(currency->id);

	Settings::GetInstance().SetConvertCurrency(_convertCurrenciesCheckBox->IsChecked());
	Settings::GetInstance().SetLoadExchangeRates(_loadExchangeRatesCheckBox->IsChecked());
	Settings::GetInstance().SetShowCreditFunds(_showCreditFundsCheckBox->IsChecked());
	Settings::GetInstance().SetShowBaseCurrencyAmount(_showBaseCurrencyAmountCheckBox->IsChecked());

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