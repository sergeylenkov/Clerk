#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height, Icons& icons) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));
	
	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(80, -1));
	wxSize fieldSize = FromDIP(wxSize(100, -1));
	
	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText* nameLabel = new wxStaticText(mainPanel, wxID_ANY, _("Name:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL);

	_nameField = new wxTextCtrl(mainPanel, wxID_ANY, "");
	horizontalSizer->Add(_nameField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* typeLabel = new wxStaticText(mainPanel, wxID_ANY, _("Type:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(typeLabel, 0, wxALIGN_CENTER_VERTICAL);

	_typeList = new wxComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, fieldSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_typeList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *currencyLabel = new wxStaticText(mainPanel, wxID_ANY, _("Currency:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(currencyLabel, 0, wxALIGN_CENTER_VERTICAL);

	_currencyList = new wxComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(150, -1)), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_currencyList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* iconLabel = new wxStaticText(mainPanel, wxID_ANY, _("Icon:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(iconLabel, 0, wxALIGN_CENTER_VERTICAL);

	_iconList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(50, -1)), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_iconList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* amountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Amount:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL);
	
	_amountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, fieldSize);
	horizontalSizer->Add(_amountField);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* creditLabel = new wxStaticText(mainPanel, wxID_ANY, _("Credit Limit:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(creditLabel, 0, wxALIGN_CENTER_VERTICAL);

	_creditField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, fieldSize);
	horizontalSizer->Add(_creditField);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *noteLabel = new wxStaticText(mainPanel, wxID_ANY, _("Note:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(noteLabel);

	_noteField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	horizontalSizer->Add(_noteField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 1, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* okButton = new wxButton(mainPanel, wxID_ANY, _("OK"));
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	wxButton* cancelButton = new wxButton(mainPanel, wxID_ANY, _("Cancel"));
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, indent * 2);

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);	

	_typeList->Bind(wxEVT_COMBOBOX, &AccountDialog::OnTypeSelect, this);
	_iconList->Bind(wxEVT_COMBOBOX, &AccountDialog::OnIconSelect, this);
	_currencyList->Bind(wxEVT_COMBOBOX, &AccountDialog::OnCurrencySelect, this);
	_amountField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnAmountKillFocus, this);
	_creditField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnCreditKillFocus, this);
	_nameField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnNameKillFocus, this);
	_noteField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnNoteKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &AccountDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AccountDialog::OnCancel, this);

	Bind(wxEVT_CHAR_HOOK, &AccountDialog::OnKeyDown, this);
}

AccountDialog::~AccountDialog() {
	delete _viewModel;
}

void AccountDialog::SetViewModel(AccountViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](AccountViewModelField field) {
		if (field == AccountViewModelField::Amount) {
			_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
		}

		if (field == AccountViewModelField::CreditLimit) {
			_creditField->SetValue(Format::Amount(_viewModel->GetCreditLimit()));
		}

		if (field == AccountViewModelField::Type) {
			_creditField->SetEditable(_viewModel->GetType() == AccountType::Deposit);
		}
	};

	Update();

	_nameField->SetFocus();
}

void AccountDialog::Update() {
	auto types = _viewModel->GetTypes();	

	_typeList->Clear();

	for (auto& type : types) {
		_typeList->AppendString(type);
	}

	_typeList->SetSelection(static_cast<int>(_viewModel->GetType()));

	_currencyList->Clear();

	auto currencies = _viewModel->GetCurrencies();

	for (auto& currency : currencies) {
		wxString name = wxString::Format("%s (%s)", currency->shortName, currency->name);
		_currencyList->AppendString(name);
	}

	_currencyList->SetSelection(_viewModel->GetCurrencyIndex());

	for (int i = 0; i < _icons.GetAccountIconsCount() - 1; i++) {
		_iconList->Append("", *_icons.GetAccountIcon(i));
	}

	_iconList->SetSelection(_viewModel->GetIconIndex());

	_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
	_creditField->SetValue(Format::Amount(_viewModel->GetCreditLimit()));
	_nameField->SetValue(_viewModel->GetName());
	_noteField->SetValue(_viewModel->GetNote());	

	_amountField->SetEditable(_viewModel->IsNew());
	_creditField->SetEditable(_viewModel->GetType() == AccountType::Deposit);
}

void AccountDialog::OnAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void AccountDialog::OnCreditKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetCreditLimit(_creditField->GetFloatValue());
}

void AccountDialog::OnNameKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetName(_nameField->GetValue());
}

void AccountDialog::OnNoteKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetNote(_noteField->GetValue());
}

void AccountDialog::OnTypeSelect(wxCommandEvent& event) {
	int index = _typeList->GetSelection();
	_viewModel->SetType(static_cast<AccountType>(index));
}

void AccountDialog::OnIconSelect(wxCommandEvent& event) {
	int index = _iconList->GetSelection();
	_viewModel->SetIconIndex(index);
}

void AccountDialog::OnCurrencySelect(wxCommandEvent& event) {
	int index = _currencyList->GetSelection();
	auto currencies = _viewModel->GetCurrencies();

	_viewModel->SetCurrency(currencies[index]);
}

void AccountDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}

void AccountDialog::OnOK(wxCommandEvent& event) {
	_viewModel->Save();

	Close();
}

void AccountDialog::OnCancel(wxCommandEvent& event) {
	Close();
}