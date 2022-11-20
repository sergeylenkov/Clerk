#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height, Icons& icons) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));
	
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	
	nameLabel = new wxStaticText(mainPanel, wxID_ANY, "Name:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	typeLabel = new wxStaticText(mainPanel, wxID_ANY, "Type:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(typeLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	typeList = new wxComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(typeList, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	currencyLabel = new wxStaticText(mainPanel, wxID_ANY, "Currency:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(currencyLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	currencyList = new wxComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(currencyList, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	iconLabel = new wxStaticText(mainPanel, wxID_ANY, "Icon:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(iconLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	iconList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(iconList, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	amountLabel = new wxStaticText(mainPanel, wxID_ANY, "Amount:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	
	amountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1));
	horizontalSizer->Add(amountField, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	noteLabel = new wxStaticText(mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(noteLabel, 0, wxALIGN_TOP | wxALL, 5);

	noteField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	horizontalSizer->Add(noteField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 1, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(mainPanel, wxID_ANY, "OK", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	cancelButton = new wxButton(mainPanel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);	
	this->SetDoubleBuffered(true);

	amountField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnAmountKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &AccountDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AccountDialog::OnCancel, this);
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

	nameField->SetFocus();
}

void AccountDialog::Update() {
	auto types = _viewModel->GetTypes();	

	typeList->Clear();

	for (auto& type : types) {
		typeList->AppendString(type);
	}

	typeList->SetSelection(static_cast<int>(_viewModel->GetType()));

	currencyList->Clear();

	auto currencies = _viewModel->GetCurrencies();

	for (auto& currency : currencies) {
		wxString name = wxString::Format("%s (%s)", currency->shortName, currency->name);
		currencyList->AppendString(name);
	}

	currencyList->SetSelection(_viewModel->GetCurrencyIndex());

	for (int i = 0; i < _icons.GetImageList()->GetImageCount(); i++) {
		iconList->Append("", *_icons.GetBitmapForIcon(i));
	}

	iconList->SetSelection(_viewModel->GetIconIndex());

	nameField->SetValue(_viewModel->GetName());
	noteField->SetValue(_viewModel->GetNote());
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

	_viewModel->SetAmount(amountField->GetFloatValue());
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