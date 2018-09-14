#include "SchedulerFrame.h"

SchedulerFrame::SchedulerFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	nameLabel = new wxStaticText(this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(40, -1), 0);
	nameLabel->Wrap(-1);
	bSizer5->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer5, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *bSizer6 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Recurrence pattern")), wxHORIZONTAL);

	wxPanel *m_panel5 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, -1), wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxVERTICAL);

	dailyButton = new wxRadioButton(m_panel5, wxID_ANY, wxT("Daily"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(dailyButton, 0, wxALL, 5);

	weeklyButton = new wxRadioButton(m_panel5, wxID_ANY, wxT("Weekly"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(weeklyButton, 0, wxALL, 5);

	monthlyButton = new wxRadioButton(m_panel5, wxID_ANY, wxT("Monthly"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(monthlyButton, 0, wxALL, 5);

	yearlyButton = new wxRadioButton(m_panel5, wxID_ANY, wxT("Yearly"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(yearlyButton, 0, wxALL, 5);


	m_panel5->SetSizer(bSizer14);
	m_panel5->Layout();
	bSizer6->Add(m_panel5, 0, wxALL, 5);

	wxStaticLine *m_staticline2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer6->Add(m_staticline2, 0, wxEXPAND | wxALL, 5);

	wxPanel *m_panel6 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	bSizer6->Add(m_panel6, 1, wxEXPAND | wxALL, 5);


	bSizer1->Add(bSizer6, 1, wxEXPAND | wxALL, 10);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(this, wxID_ANY, wxT("From:"), wxDefaultPosition, wxSize(40, -1), 0);
	fromLabel->Wrap(-1);
	bSizer9->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	bSizer9->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), 0);
	bSizer9->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(this, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	fromAmountLabel->Wrap(-1);
	bSizer9->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer9, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(this, wxID_ANY, wxT("To:"), wxDefaultPosition, wxSize(40, -1), 0);
	toLabel->Wrap(-1);
	bSizer10->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	bSizer10->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), 0);
	bSizer10->Add(toAmountField, 0, wxALL, 5);

	toAmountLabel = new wxStaticText(this, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	toAmountLabel->Wrap(-1);
	bSizer10->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer10, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(this, wxID_ANY, wxT("Tags:"), wxDefaultPosition, wxSize(40, -1), 0);
	tagsLabel->Wrap(-1);
	bSizer11->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer11->Add(tagsField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer11, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer2, 0, wxALIGN_RIGHT | wxALL, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	/*wxBoxSizer *statsizer = new wxStaticBoxSizer(
		new wxStaticBox(p, wxID_ANY, wxT("A wxStaticBoxSizer")), wxVERTICAL);*/

	/*wxString allowedChars[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " " };
	wxArrayString chars(13, allowedChars);
	wxTextValidator amountValidator(wxFILTER_INCLUDE_CHAR_LIST);
	amountValidator.SetIncludes(chars);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *lineSizer = new wxBoxSizer(wxHORIZONTAL);

	nameLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);	

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(mainPanel, wxID_ANY, "From:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	lineSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	lineSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(mainPanel, wxID_ANY, "To:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	lineSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	lineSizer->Add(toAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(mainPanel, wxID_ANY, "Tags:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(tagsField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	noteLabel = new wxStaticText(mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(noteLabel, 0, wxALL, 5);

	noteField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	lineSizer->Add(noteField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(lineSizer, 1, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(mainPanel, wxID_ANY, "OK", wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(mainPanel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	panelSizer->Add(lineSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();*/

	this->Centre(wxBOTH);

	tagsPopup = new TagsPopup(this);
	tagsPopup->OnSelectTag = std::bind(&SchedulerFrame::OnSelectTag, this);

	okButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &SchedulerFrame::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &SchedulerFrame::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnFromAmountKillFocus, this);
	toAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnToAmountKillFocus, this);
	tagsField->Bind(wxEVT_KEY_UP, &SchedulerFrame::OnTextChanged, this);
	tagsField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnTagsKillFocus, this);

	fromValue = 0;
	toValue = 0;

	dailyButton->SetValue(true);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Receipt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Debt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Credit))
	{
		accounts.push_back(account);
	}

	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	UpdateFromList();
	SelectFromAccount(0);

	UpdateToList(fromAccount);
	SelectToAccount(0);
}

SchedulerFrame::~SchedulerFrame() {	
	delete accountsImageList;
	delete tagsPopup;
}

void SchedulerFrame::SetScheduler(std::shared_ptr<Scheduler> scheduler) {
	this->scheduler = scheduler;

	/*fromAmountField->SetValue(wxString::Format("%.2f", transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", transaction->toAmount));
	tagsField->SetValue(*transaction->tags);
	noteField->SetValue(*transaction->note);	

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i]);

			break;
		}
	}

	if (transaction->toAccountId != -1) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (transaction->toAccountId == toAccounts[i]->id) {
				SelectToAccount(i);
				break;
			}
		}
	}
	else {
		SelectToAccount(0);
	}*/

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void SchedulerFrame::UpdateFromList() {
	for each (auto account in accounts) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			int icon = 0;

			if (account->iconId < accountsImageList->GetImageCount()) {
				icon = account->iconId;
			}

			fromList->Append(*account->name, accountsImageList->GetBitmap(icon));

			fromAccounts.push_back(account);
		}
	}
}

void SchedulerFrame::UpdateToList(std::shared_ptr<Account> account) {
	toList->Clear();
	toAccounts.clear();

	for each (auto toAccount in accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == AccountTypes::Receipt) {
			if (toAccount->type == AccountTypes::Deposit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
		else if (account->type == AccountTypes::Deposit) {
			if (toAccount->type == AccountTypes::Deposit || toAccount->type == AccountTypes::Expens || toAccount->type == AccountTypes::Debt || toAccount->type == AccountTypes::Credit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
	}
}

void SchedulerFrame::SelectFromAccount(int index) {
	auto account = fromAccounts[index];

	fromList->Select(index);
	fromAmountLabel->SetLabel(*account->currency->shortName);
	fromAccount = account;
}

void SchedulerFrame::SelectToAccount(int id) {
	auto account = toAccounts[id];

	toList->Select(id);
	toAmountLabel->SetLabel(*account->currency->shortName);

	toAccount = account;
}

void SchedulerFrame::SelectToAccount(std::shared_ptr<Account> account) {
	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (toAccounts[i]->id == account->id) {
			SelectToAccount(i);
			return;
		}
	}

	SelectToAccount(0);
}

void SchedulerFrame::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());

	UpdateToList(fromAccount);
	SelectToAccount(toAccount);
}

void SchedulerFrame::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());
}

void SchedulerFrame::OnOK(wxCommandEvent &event) {
	/*double val;

	wxString value = fromAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);

	fromValue = val;

	value = toAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);

	toValue = val;

	transaction->fromAccountId = fromAccounts[fromList->GetSelection()]->id;
	transaction->toAccountId = toAccounts[toList->GetSelection()]->id;
	transaction->fromAmount = fromValue;
	transaction->toAmount = toValue;
	transaction->note = make_shared<wxString>(noteField->GetValue());
	transaction->tags = make_shared<wxString>(tagsField->GetValue());

	transaction->Save();*/

	Close();

	if (OnClose) {
		OnClose();
	}
}

void SchedulerFrame::OnCancel(wxCommandEvent &event) {
	Close();
}

void SchedulerFrame::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(fromAmountField->GetValue());
	fromAmountField->SetValue(stringAmount);

	int fromCurrencyId = fromAccounts[fromList->GetSelection()]->currency->id;
	int toCurrencyId = toAccounts[toList->GetSelection()]->currency->id;
	double val;

	toAmountField->GetValue().ToDouble(&val);

	if (val == 0 && fromCurrencyId == toCurrencyId) {
		toAmountField->SetValue(fromAmountField->GetValue());
	}
}

void SchedulerFrame::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(toAmountField->GetValue());
	toAmountField->SetValue(stringAmount);
}

void SchedulerFrame::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		tagsPopup->Hide();
	}
	else if (event.GetKeyCode() == WXK_UP) {
		tagsPopup->SelectPrev();
		event.StopPropagation();
	}
	else if (event.GetKeyCode() == WXK_DOWN) {
		tagsPopup->SelectNext();
	}
	else if (event.GetKeyCode() == WXK_RETURN) {
		AddTag();
		tagsPopup->Hide();
	}
	else {
		wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
		vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {
			auto tags = DataHelper::GetInstance().GetTagsBySearch(tokens.back());

			if (!tokens.empty()) {
				tagsPopup->Update(tags);

				wxPoint pos = tagsField->GetScreenPosition();
				wxSize size = tagsField->GetSize();

				tagsPopup->Position(wxPoint(pos.x - 200, pos.y - 200 + size.GetHeight()), wxSize(200, 200));
				tagsPopup->Show();
			}
			else {
				tagsPopup->Hide();
			}
		}
	}

	event.Skip();
}

void SchedulerFrame::OnTagsKillFocus(wxFocusEvent& event) {
	tagsPopup->Hide();
	event.Skip();
}

void SchedulerFrame::OnSelectTag() {
	AddTag();
	tagsPopup->Hide();
}

void SchedulerFrame::AddTag() {
	wxString tag = tagsPopup->GetSelectedTag();
	wxString result = "";

	wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
	vector<wxString> tokens;

	while (tokenizer.HasMoreTokens()) {
		wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
		tokens.push_back(token);
	}

	for (unsigned int i = 0; i < tokens.size() - 1; i++) {
		result.Append(tokens[i]);
		result.Append(", ");
	}

	result.Append(tag);

	tagsField->SetValue(result);
	tagsField->SetInsertionPointEnd();
}

wxString SchedulerFrame::ClearAmountValue(wxString &value) {
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);

	return value;
}