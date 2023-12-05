#include "TransactionDialog.h"

TransactionDialog::TransactionDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons, DataContext& context):
	wxFrame(parent, wxID_ANY, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
	_icons(icons), _context(context) {
	SetBackgroundColour(wxColor(* wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	int indent = this->FromDIP(5);
	int bottomIndent = this->FromDIP(15);
	wxSize labelSize = this->FromDIP(wxSize(40, -1));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* fromLabel = new wxStaticText(_mainPanel, wxID_ANY, "From:", wxDefaultPosition, labelSize);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL, indent);

	_fromList = new wxBitmapComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_fromList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_fromAmountField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, this->FromDIP(wxSize(80, -1)));
	horizontalSizer->Add(_fromAmountField, 0, wxALIGN_CENTER_VERTICAL);

	_fromAmountLabel = new wxStaticText(_mainPanel, wxID_ANY, "RUB");
	horizontalSizer->Add(_fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* toLabel = new wxStaticText(_mainPanel, wxID_ANY, "To:", wxDefaultPosition, labelSize);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL, indent);

	_toList = new wxBitmapComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_toList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_toAmountField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, this->FromDIP(wxSize(80, -1)));
	horizontalSizer->Add(_toAmountField, 0, wxALIGN_CENTER_VERTICAL);

	_toAmountLabel = new wxStaticText(_mainPanel, wxID_ANY, "RUB");
	horizontalSizer->Add(_toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);
	
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* dateLabel = new wxStaticText(_mainPanel, wxID_ANY, "Date:", wxDefaultPosition, labelSize);
	horizontalSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL, indent);

	_datePicker = new wxDatePickerCtrl(_mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(_datePicker);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* tagsLabel = new wxStaticText(_mainPanel, wxID_ANY, "Tags:", wxDefaultPosition, labelSize);
	horizontalSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL, indent);

	_tagsFieldNew = new TagsField(_mainPanel, context.GetTagsService());
	_tagsFieldNew->OnChange = [&]() {
		_viewModel->SetTags(_tagsFieldNew->GetTags());
	};

	horizontalSizer->Add(_tagsFieldNew, 1);
	//_tagsField = new wxTextCtrl(_mainPanel, wxID_ANY);
	//horizontalSizer->Add(_tagsField, 1);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* noteLabel = new wxStaticText(_mainPanel, wxID_ANY, "Note:", wxDefaultPosition, labelSize);
	horizontalSizer->Add(noteLabel);

	_noteField = new wxTextCtrl(_mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	horizontalSizer->Add(_noteField, 1, wxEXPAND, indent);

	panelSizer->Add(horizontalSizer, 1, wxEXPAND | wxBOTTOM, bottomIndent);
	
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_okButton = new wxButton(_mainPanel, wxID_ANY, "OK");
	horizontalSizer->Add(_okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	_cancelButton = new wxButton(_mainPanel, wxID_ANY, "Cancel");
	horizontalSizer->Add(_cancelButton, 0, wxALIGN_CENTER_VERTICAL);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT);

	_mainPanel->SetSizer(panelSizer);
	_mainPanel->Layout();

	panelSizer->Fit(_mainPanel);

	mainSizer->Add(_mainPanel, 1, wxEXPAND | wxALL, indent * 2);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
	this->SetDoubleBuffered(true);

	/*_tagsPopup = new TagsPopup(this);
	_tagsPopup->OnSelectTag = std::bind(&TransactionDialog::OnSelectTag, this);*/

	_okButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnOK, this);
	_cancelButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnCancel, this);

	_fromList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnFromAccountSelect, this);
	_toList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnToAccountSelect, this);
	_fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnFromAmountKillFocus, this);
	_toAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnToAmountKillFocus, this);
	_datePicker->Bind(wxEVT_DATE_CHANGED, &TransactionDialog::OnDateChanged, this);
	_noteField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnNoteKillFocus, this);

	this->Bind(wxEVT_CHAR_HOOK, &TransactionDialog::OnKeyDown, this);
}

TransactionDialog::~TransactionDialog() {
	delete _viewModel;
	delete _mainPanel;
}

void TransactionDialog::SetViewModel(TransactionViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](TransactionViewModelField field) {
		if (field == TransactionViewModelField::FromAmount || field == TransactionViewModelField::ToAmount) {
			_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
			_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
		}

		if (field == TransactionViewModelField::FromAccount) {
			UpdateToList();
			SelectToAccount(_viewModel->GetToAccountIndex());
		}

		if (field == TransactionViewModelField::ToAccount) {
			UpdateFromList();
			SelectFromAccount(_viewModel->GetFromAccountIndex());
		}
	};

	Update();
	
	_fromAmountField->SetFocus();
	_fromAmountField->SelectAll();
}

void TransactionDialog::Update() {
	UpdateFromList();
	UpdateToList();

	SelectFromAccount(_viewModel->GetFromAccountIndex());
	SelectToAccount(_viewModel->GetToAccountIndex());

	_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
	_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
	_tagsFieldNew->SetTags(_viewModel->GetTags());
	_datePicker->SetValue(_viewModel->GetDate());
	_noteField->SetValue(_viewModel->GetNote());
}

void TransactionDialog::UpdateFromList() {
	auto accounts = _viewModel->GetFromAccounts();

	_fromList->Clear();

	for (auto& account : accounts) {
		_fromList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void TransactionDialog::UpdateToList() {
	auto accounts = _viewModel->GetToAccounts();

	_toList->Clear();

	for (auto& account : accounts) {
		_toList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void TransactionDialog::SelectFromAccount(int index) {
	auto account = _viewModel->GetFromAccounts()[index];

	_fromList->Select(index);
	_fromAmountLabel->SetLabel(account->currency->shortName);
}

void TransactionDialog::SelectToAccount(int index) {
	auto account = _viewModel->GetToAccounts()[index];

	_toList->Select(index);
	_toAmountLabel->SetLabel(account->currency->shortName);
}

void TransactionDialog::OnFromAccountSelect(wxCommandEvent &event) {
	int index = _fromList->GetSelection();
	_viewModel->SetFromAccount(index);
}

void TransactionDialog::OnToAccountSelect(wxCommandEvent &event) {
	int index = _toList->GetSelection();
	_viewModel->SetToAccount(index);
}

void TransactionDialog::OnDateChanged(wxDateEvent& event) {	
	_viewModel->SetDate(_datePicker->GetValue());
}

void TransactionDialog::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	_viewModel->SetFromAmount(_fromAmountField->GetFloatValue());
}

void TransactionDialog::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetToAmount(_toAmountField->GetFloatValue());
}

void TransactionDialog::OnNoteKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetNote(_noteField->GetValue());
}

void TransactionDialog::OnOK(wxCommandEvent& event) {
	_viewModel->Save();

	Close();
}

void TransactionDialog::OnCancel(wxCommandEvent& event) {
	Close();
}

void TransactionDialog::OnKeyDown(wxKeyEvent& event) {
	if ((int)event.GetKeyCode() == WXK_ESCAPE) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}