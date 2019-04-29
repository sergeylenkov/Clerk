#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	currencyLabel = new wxStaticText(this, wxID_ANY, wxT("Base Currency:"), wxDefaultPosition, wxSize(-1, -1), 0);	
	horizontalSizer->Add(currencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	currencyList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(currencyList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	mainSizer->Add(0, 0, 1, wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
		
	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	int index = 0;
	int i = 0;

	for (auto currency : DataHelper::GetInstance().GetCurrencies())
	{
		currencies.push_back(currency);

		wxString name = wxString::Format("%s (%s)", currency->shortName->c_str(), currency->name->c_str());
		currencyList->AppendString(name);

		if (baseCurrencyId == currency->id) {
			index = i;
		}

		i++;
	}

	currencyList->SetSelection(index);

	okButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &PreferencesDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &PreferencesDialog::OnKeyDown, this);
}

void PreferencesDialog::OnOK(wxCommandEvent &event) {
	auto currency = currencies[currencyList->GetSelection()];
	Settings::GetInstance().SetBaseCurrencyId(currency->id);

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