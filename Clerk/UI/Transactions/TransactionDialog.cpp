#include "TransactionDialog.h"

TransactionDialog::TransactionDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(* wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(mainPanel, wxID_ANY, "From:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1));
	horizontalSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(mainPanel, wxID_ANY, "To:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1));
	horizontalSizer->Add(toAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);
	
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	dateLabel = new wxStaticText(mainPanel, wxID_ANY, "Date:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(datePicker, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(mainPanel, wxID_ANY, "Tags:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(tagsField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	noteLabel = new wxStaticText(mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(noteLabel, 0, wxALL, 5);

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

	_tagsPopup = new TagsPopup(this);	
	_tagsPopup->OnSelectTag = std::bind(&TransactionDialog::OnSelectTag, this);

	okButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &TransactionDialog::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &TransactionDialog::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnFromAmountKillFocus, this);
	toAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnToAmountKillFocus, this);
	tagsField->Bind(wxEVT_KEY_UP, &TransactionDialog::OnTextChanged, this);
	tagsField->Bind(wxEVT_KILL_FOCUS, &TransactionDialog::OnTagsKillFocus, this);
	tagsField->Bind(wxEVT_CHAR_HOOK, &TransactionDialog::OnTagsKeyDown, this);
	this->Bind(wxEVT_CHAR_HOOK, &TransactionDialog::OnKeyDown, this);
}

TransactionDialog::~TransactionDialog() {
	delete _viewModel;
	delete _tagsPopup;	
}

void TransactionDialog::SetTransaction(std::shared_ptr<TransactionViewModel> transaction)  {
	_transaction = transaction;

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void TransactionDialog::Update() {
	UpdateFromList();
	UpdateToList();

	SelectFromAccount(_viewModel->GetFromAccountIndex());
	SelectToAccount(_viewModel->GetToAccountIndex());

	fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
	toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
	tagsField->SetValue(_viewModel->GetTagsString());
	datePicker->SetValue(_viewModel->GetDate());
}

void TransactionDialog::UpdateToList() {
	auto accounts = _viewModel->GetFromAccounts();

	fromList->Clear();

	for (auto& account : accounts) {
		int iconIndex = _icons.GetIconForAccount(account->icon);
		fromList->Append(account->name, _icons.GetBitmapForIcon(iconIndex));
	}

	accounts = _viewModel->GetToAccounts();
}

void TransactionDialog::UpdateFromList() {
	auto accounts = _viewModel->GetToAccounts();

	toList->Clear();

	for (auto& account : accounts) {
		int iconIndex = _icons.GetIconForAccount(account->icon);
		toList->Append(account->name, _icons.GetBitmapForIcon(iconIndex));
	}
}

void TransactionDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == WXK_ESCAPE) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}

void TransactionDialog::SelectFromAccount(int index) {
	auto account = _viewModel->GetFromAccounts()[index];

	fromList->Select(index);
	fromAmountLabel->SetLabel(*account->currency->shortName);
}

void TransactionDialog::SelectToAccount(int index) {
	auto account = _viewModel->GetToAccounts()[index];

	toList->Select(index);
	toAmountLabel->SetLabel(*account->currency->shortName);
}

void TransactionDialog::OnFromAccountSelect(wxCommandEvent &event) {
	int index = fromList->GetSelection();
	_viewModel->SetFromAccount(index);
}

void TransactionDialog::OnToAccountSelect(wxCommandEvent &event) {
	int index = fromList->GetSelection();
	_viewModel->SetToAccount(index);
}

void TransactionDialog::OnOK(wxCommandEvent &event) {
	_viewModel->Save();

	Close();
}

void TransactionDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void TransactionDialog::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	_viewModel->SetFromAmount(fromAmountField->GetFloatValue());
}

void TransactionDialog::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetToAmount(toAmountField->GetFloatValue());
}

void TransactionDialog::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		_viewModel->SetTagsString(tagsField->GetValue());
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
		wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
		std::vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {			
			auto tags = _viewModel->SearchTagsByString(tokens.back());

			if (tags.size() > 0) {
				_tagsPopup->Update(tags);

				wxPoint pos = tagsField->GetScreenPosition();
				wxSize size = tagsField->GetSize();
				
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

	tagsField->SetInsertionPointEnd();
}