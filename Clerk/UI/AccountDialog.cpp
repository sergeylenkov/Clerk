#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height, Icons& icons) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));
	
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText* nameLabel = new wxStaticText(_mainPanel, wxID_ANY, "Name:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_nameField = new wxTextCtrl(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(_nameField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* typeLabel = new wxStaticText(_mainPanel, wxID_ANY, "Type:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(typeLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_typeList = new wxComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_typeList, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *currencyLabel = new wxStaticText(_mainPanel, wxID_ANY, "Currency:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(currencyLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_currencyList = new wxComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_currencyList, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* iconLabel = new wxStaticText(_mainPanel, wxID_ANY, "Icon:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(iconLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_iconList = new wxBitmapComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_iconList, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* amountLabel = new wxStaticText(_mainPanel, wxID_ANY, "Amount:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	
	_amountField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(100, -1));
	horizontalSizer->Add(_amountField, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* creditLabel = new wxStaticText(_mainPanel, wxID_ANY, "Credit Limit:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(creditLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_creditField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(100, -1));
	horizontalSizer->Add(_creditField, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *noteLabel = new wxStaticText(_mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(80, -1), 0);
	horizontalSizer->Add(noteLabel, 0, wxALIGN_TOP | wxALL, 5);

	_noteField = new wxTextCtrl(_mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	horizontalSizer->Add(_noteField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 1, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_okButton = new wxButton(_mainPanel, wxID_ANY, "OK", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(_okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_cancelButton = new wxButton(_mainPanel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(_cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	_mainPanel->SetSizer(panelSizer);
	_mainPanel->Layout();

	panelSizer->Fit(_mainPanel);

	mainSizer->Add(_mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);	
	this->SetDoubleBuffered(true);

	_amountField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnAmountKillFocus, this);
	_creditField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnCreditKillFocus, this);
	_okButton->Bind(wxEVT_BUTTON, &AccountDialog::OnOK, this);
	_cancelButton->Bind(wxEVT_BUTTON, &AccountDialog::OnCancel, this);
	this->Bind(wxEVT_CHAR_HOOK, &AccountDialog::OnKeyDown, this);
}

AccountDialog::~AccountDialog() {
	delete _viewModel;
}

void AccountDialog::SetViewModel(AccountViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](AccountViewModelField field) {
		Update();
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

	for (int i = 0; i < _icons.GetImageList()->GetImageCount(); i++) {
		_iconList->Append("", *_icons.GetBitmapForIcon(i));
	}

	_iconList->SetSelection(_viewModel->GetIconIndex());

	_nameField->SetValue(_viewModel->GetName());
	_noteField->SetValue(_viewModel->GetNote());
	_creditField->SetValue(Format::Amount(_viewModel->GetCreditLimit()));
}


void AccountDialog::OnOK(wxCommandEvent &event) {
	_viewModel->Save();

	Close();
}

void AccountDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void AccountDialog::OnAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void AccountDialog::OnCreditKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetCreditLimit(_amountField->GetFloatValue());
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