#include "TransactionDialog.h"

TransactionDialog::TransactionDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(* wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* fromLabel = new wxStaticText(_mainPanel, wxID_ANY, "From:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_fromList = new wxBitmapComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_fromAmountField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1));
	horizontalSizer->Add(_fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_fromAmountLabel = new wxStaticText(_mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(_fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* toLabel = new wxStaticText(_mainPanel, wxID_ANY, "To:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_toList = new wxBitmapComboBox(_mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_toAmountField = new AmountField(_mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1));
	horizontalSizer->Add(_toAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_toAmountLabel = new wxStaticText(_mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(_toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);
	
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* dateLabel = new wxStaticText(_mainPanel, wxID_ANY, "Date:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_datePicker = new wxDatePickerCtrl(_mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(_datePicker, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* tagsLabel = new wxStaticText(_mainPanel, wxID_ANY, "Tags:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_tagsField = new wxTextCtrl(_mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(_tagsField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* noteLabel = new wxStaticText(_mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(noteLabel, 0, wxALL, 5);

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

	_tagsPopup = new TagsPopup(this);	
	_tagsPopup->OnSelectTag = std::bind(&TransactionDialog::OnSelectTag, this);

	_okButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnOK, this);
	_cancelButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnCancel, this);

	_fromList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnFromAccountSelect, this);
	_toList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnToAccountSelect, this);
	_fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnFromAmountKillFocus, this);
	_toAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnToAmountKillFocus, this);
	_tagsField->Bind(wxEVT_KEY_UP, &TransactionDialog::OnTextChanged, this);
	_tagsField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnTagsKillFocus, this);
	_tagsField->Bind(wxEVT_CHAR_HOOK, &TransactionDialog::OnTagsKeyDown, this);
	this->Bind(wxEVT_CHAR_HOOK, &TransactionDialog::OnKeyDown, this);
}

TransactionDialog::~TransactionDialog() {
	delete _viewModel;
	delete _tagsPopup;
	delete _mainPanel;
	wxLogDebug("~TransactionDialog");
}

void TransactionDialog::SetViewModel(TransactionViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](TransactionViewModelField field) {
		if (field == TransactionViewModelField::FromAmount || field == TransactionViewModelField::ToAmount) {
			_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
			_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
		}

		if (field == TransactionViewModelField::Tags) {
			_tagsField->SetValue(_viewModel->GetTagsString());
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
	_tagsField->SetValue(_viewModel->GetTagsString());
	_datePicker->SetValue(_viewModel->GetDate());
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

void TransactionDialog::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	_viewModel->SetFromAmount(_fromAmountField->GetFloatValue());
}

void TransactionDialog::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetToAmount(_toAmountField->GetFloatValue());
}

void TransactionDialog::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		_viewModel->SetTagsString(_tagsField->GetValue());
		_tagsPopup->Hide();
	} else if (event.GetKeyCode() == WXK_UP) {
		_tagsPopup->SelectPrev();
		event.StopPropagation();
	} else if (event.GetKeyCode() == WXK_DOWN) {
		_tagsPopup->SelectNext();
	} else if (event.GetKeyCode() == WXK_RETURN) {		
		AddTag();
		_tagsPopup->Hide();
	} else {
		wxStringTokenizer tokenizer(_tagsField->GetValue(), ",");
		std::vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {			
			auto tags = _viewModel->SearchTagsByString(tokens.back());

			if (tags.size() > 0) {
				_tagsPopup->Update(tags);

				wxPoint pos = _tagsField->GetScreenPosition();
				wxSize size = _tagsField->GetSize();
				
				_tagsPopup->Position(wxPoint(pos.x - 200, pos.y - 200 + size.GetHeight()), wxSize(200, 200));
				_tagsPopup->Show();
			}
			else {
				_tagsPopup->Hide();
			}
		}
	}

	event.Skip();
}

void TransactionDialog::OnTagsKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == WXK_ESCAPE && _tagsPopup->IsShown()) {
		event.StopPropagation();		 
	}
	else {
		event.Skip();
	}
}

void TransactionDialog::OnTagsKillFocus(wxFocusEvent& event) {
	_tagsPopup->Hide();
	event.Skip();
}

void TransactionDialog::OnSelectTag() {
	AddTag();
	_tagsPopup->Hide();	
}

void TransactionDialog::AddTag() {
	auto tag = _tagsPopup->GetSelectedTag();
	_viewModel->AddTag(tag);

	_tagsField->SetInsertionPointEnd();
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