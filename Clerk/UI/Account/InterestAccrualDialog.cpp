#include "InterestAccrualDialog.h"

InterestAccrualDialog::InterestAccrualDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize fieldSize = FromDIP(wxSize(90, -1));

	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText* amountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Accrue"));
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_typeList = new wxComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(80, -1)), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	horizontalSizer->Add(_typeList, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_amountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, fieldSize);
	horizontalSizer->Add(_amountField, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_percentField = new AmountField(mainPanel, wxID_ANY, "0.0", wxDefaultPosition, fieldSize);
	horizontalSizer->Add(_percentField, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_currencyLabel = new wxStaticText(mainPanel, wxID_ANY, "");
	horizontalSizer->Add(_currencyLabel, 0, wxALIGN_CENTER_VERTICAL);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_balanceLabel = new wxStaticText(mainPanel, wxID_ANY, "");
	horizontalSizer->Add(_balanceLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

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

	_typeList->Bind(wxEVT_COMBOBOX, &InterestAccrualDialog::OnTypeSelect, this);
	_amountField->Bind(wxEVT_KILL_FOCUS, &InterestAccrualDialog::OnAmountKillFocus, this);
	_percentField->Bind(wxEVT_KILL_FOCUS, &InterestAccrualDialog::OnPercentKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &InterestAccrualDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &InterestAccrualDialog::OnCancel, this);

	Bind(wxEVT_CHAR_HOOK, &InterestAccrualDialog::OnKeyDown, this);
}

InterestAccrualDialog::~InterestAccrualDialog() {
	delete _viewModel;
}

void InterestAccrualDialog::SetViewModel(InterestAccrualViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](InterestAccrualViewModelField field) {
		if (field == InterestAccrualViewModelField::Amount) {
			_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
			UpdateBalance();
		}

		if (field == InterestAccrualViewModelField::Percent) {
			_percentField->SetValue(Format::Amount(_viewModel->GetPercent()));
			UpdateBalance();
		}

		if (field == InterestAccrualViewModelField::Type) {
			UpdateType();
		}
	};

	Update();
	UpdateBalance();

	_amountField->SetFocus();
}

void InterestAccrualDialog::Update() {
	_typeList->Clear();

	_typeList->AppendString(_("Amount"));
	_typeList->AppendString(_("Percent"));

	_typeList->SetSelection(static_cast<int>(_viewModel->GetType()));
	
	_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
	_percentField->SetValue(Format::Amount(_viewModel->GetPercent()));

	UpdateType();
}

void InterestAccrualDialog::UpdateType() {
	_amountField->Hide();
	_percentField->Hide();

	if (_viewModel->GetType() == AccrualType::Amount) {
		_amountField->Show();
		_currencyLabel->SetLabelText(_viewModel->GetCurrencyName());
	}
	else {
		_percentField->Show();
		_currencyLabel->SetLabelText("%");
	}

	Layout();
}

void InterestAccrualDialog::UpdateBalance() {
	if (_viewModel->GetType() == AccrualType::Amount) {
		wxString balance = wxString::Format("%s + %s = %s %s", Format::Amount(_viewModel->GetBalance()), Format::Amount(_viewModel->GetAmount()),
			Format::Amount(_viewModel->GetNewBalance()), _viewModel->GetCurrencyName());
		_balanceLabel->SetLabelText(balance);
	}
	else {
		wxString balance = wxString::Format("%s + %s%% = %s %s", Format::Amount(_viewModel->GetBalance()), Format::Amount(_viewModel->GetPercent()),
			Format::Amount(_viewModel->GetNewBalance()), _viewModel->GetCurrencyName());
		_balanceLabel->SetLabelText(balance);
	}
}

void InterestAccrualDialog::OnTypeSelect(wxCommandEvent& event) {
	int index = _typeList->GetSelection();
	_viewModel->SetType(static_cast<AccrualType>(index));
}

void InterestAccrualDialog::OnPercentKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetPercent(_percentField->GetFloatValue());
}

void InterestAccrualDialog::OnAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void InterestAccrualDialog::OnKeyDown(wxKeyEvent& event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}

void InterestAccrualDialog::OnOK(wxCommandEvent& event) {
	_viewModel->Save();

	Close();
}

void InterestAccrualDialog::OnCancel(wxCommandEvent& event) {
	Close();
}