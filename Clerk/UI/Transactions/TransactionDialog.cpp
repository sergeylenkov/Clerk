#include "TransactionDialog.h"

TransactionDialog::TransactionDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
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

	tagsPopup = new TagsPopup(this);	
	tagsPopup->OnSelectTag = std::bind(&TransactionDialog::OnSelectTag, this);

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

	fromValue = 0;
	toValue = 0;

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Receipt))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Deposit))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Virtual))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Expens))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Debt))
	{
		accounts.push_back(account);
	}

	UpdateFromList();
	SelectFromAccount(0);

	UpdateToList(fromAccount.get());
	SelectToAccount(0);
}

TransactionDialog::~TransactionDialog() {
	delete tagsPopup;	
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

void TransactionDialog::SetTransaction(std::shared_ptr<Transaction> transaction) {
	this->transaction = transaction;

	fromAmountField->SetValue(Utils::FormatAmount(transaction->fromAmount));
	toAmountField->SetValue(Utils::FormatAmount(transaction->toAmount));
	tagsField->SetValue(transaction->GetTagsString());
	noteField->SetValue(*transaction->note);
	datePicker->SetValue(*transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccount->id == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i].get());

			break;
		}
	}

	if (transaction->toAccount->id != -1) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (transaction->toAccount->id == toAccounts[i]->id) {
				SelectToAccount(i);
				break;
			}
		}
	}
	else {
		SelectToAccount(0);
	}

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void TransactionDialog::SetSplitTransaction(std::shared_ptr<Transaction> transaction) {
	auto copy = make_shared<Transaction>();

	copy->fromAccount = transaction->fromAccount;
	copy->toAccount = transaction->toAccount;
	copy->fromAmount = transaction->fromAmount;
	copy->toAmount = 0.0;
	copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

	this->transaction = copy;
	this->splitTransaction = transaction;

	fromAmountField->SetValue(Utils::FormatAmount(this->transaction->fromAmount));
	toAmountField->SetValue(Utils::FormatAmount(0.0));
	tagsField->SetValue("");
	noteField->SetValue("");
	datePicker->SetValue(*this->transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (this->transaction->fromAccount->id == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i].get());

			break;
		}
	}

	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (this->transaction->toAccount->id == toAccounts[i]->id) {
			SelectToAccount(i);
			break;
		}
	}

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void TransactionDialog::UpdateFromList() {
	for (auto account : accounts) {
		if (account->type == AccountType::Receipt || account->type == AccountType::Deposit || account->type == AccountType::Virtual) {
			int iconId = 0;

			if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
				iconId = account->iconId;
			}

			fromList->Append(*account->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

			fromAccounts.push_back(account);
		}
	}
}

void TransactionDialog::UpdateToList(Account *account) {
	toList->Clear();
	toAccounts.clear();

	for (auto toAccount : accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == AccountType::Receipt) {
			if (toAccount->type == AccountType::Deposit || toAccount->type == AccountType::Virtual) {
				int iconId = 0;

				if (toAccount->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
					iconId = toAccount->iconId;
				}

				toList->Append(*toAccount->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

				toAccounts.push_back(toAccount);
			}
		}
		else if (account->type == AccountType::Deposit || account->type == AccountType::Virtual) {
			if (toAccount->type == AccountType::Deposit || toAccount->type == AccountType::Expens
				|| toAccount->type == AccountType::Debt || toAccount->type == AccountType::Virtual) {
				int iconId = 0;

				if (toAccount->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
					iconId = toAccount->iconId;
				}

				toList->Append(*toAccount->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

				toAccounts.push_back(toAccount);
			}
		}
	}
}

void TransactionDialog::SelectFromAccount(int index) {
	auto account = fromAccounts[index];

	fromList->Select(index);
	fromAmountLabel->SetLabel(*account->currency->shortName);
	fromAccount = account;
}

void TransactionDialog::SelectToAccount(int id) {
	auto account = toAccounts[id];

	toList->Select(id);
	toAmountLabel->SetLabel(*account->currency->shortName);

	toAccount = account;
}

void TransactionDialog::SelectToAccount(Account *account) {
	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (toAccounts[i]->id == account->id) {
			SelectToAccount(i);
			return;
		}
	}

	SelectToAccount(0);
}

void TransactionDialog::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());

	UpdateToList(fromAccount.get());
	SelectToAccount(toAccount.get());
}

void TransactionDialog::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());

	Account *fromAccount = fromAccounts[fromList->GetSelection()].get();
	Account *toAccount = toAccounts[toList->GetSelection()].get();

	if (fromAccount->currency->id != toAccount->currency->id) {
		float fromValue = fromAmountField->GetFloatValue();
		float toValue = toAmountField->GetFloatValue();

		float amount = DataHelper::GetInstance().ConvertCurrency(fromAccount->currency->id, toAccount->currency->id, fromValue);
		toAmountField->SetValue(Utils::FormatAmount(amount));
	}
}

void TransactionDialog::OnOK(wxCommandEvent &event) {
	transaction->fromAccount = fromAccounts[fromList->GetSelection()];
	transaction->toAccount = toAccounts[toList->GetSelection()];
	transaction->note = make_shared<wxString>(noteField->GetValue());
	transaction->SetTagsString(tagsField->GetValue());
	transaction->paidAt = make_shared<wxDateTime>(datePicker->GetValue());	
		  
	transaction->fromAmount = fromAmountField->GetFloatValue();
	transaction->toAmount = toAmountField->GetFloatValue();

	transaction->Save();

	if (splitTransaction) {
		splitTransaction->fromAmount = splitTransaction->fromAmount - transaction->fromAmount;
		splitTransaction->toAmount = splitTransaction->toAmount - transaction->toAmount;

		splitTransaction->Save();
	}

	Close();

	if (OnClose) {
		OnClose();
	}
}

void TransactionDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void TransactionDialog::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	int fromCurrencyId = fromAccounts[fromList->GetSelection()]->currency->id;
	int toCurrencyId = toAccounts[toList->GetSelection()]->currency->id;	

	float fromValue = fromAmountField->GetFloatValue();
	float toValue = toAmountField->GetFloatValue();

	if (toValue == 0) {
		if (fromCurrencyId == toCurrencyId) {
			toAmountField->SetValue(Utils::FormatAmount(fromAmountField->GetFloatValue()));
		}

		else if (Settings::GetInstance().IsConvertCurrency()) {
			float amount = DataHelper::GetInstance().ConvertCurrency(fromCurrencyId, toCurrencyId, fromValue);
			toAmountField->SetValue(Utils::FormatAmount(amount));
		}
	}
}

void TransactionDialog::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
}

void TransactionDialog::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		tagsPopup->Hide();
	} else if (event.GetKeyCode() == WXK_UP) {
		tagsPopup->SelectPrev();
		event.StopPropagation();
	} else if (event.GetKeyCode() == WXK_DOWN) {
		tagsPopup->SelectNext();
	} else if (event.GetKeyCode() == WXK_RETURN) {		
		AddTag();
		tagsPopup->Hide();
	} else {
		wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
		vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {
			auto tags = DataHelper::GetInstance().GetTagsBySearch(tokens.back());

			if (!tokens.empty() && tags.size() > 0) {
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

void TransactionDialog::OnTagsKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == WXK_ESCAPE && tagsPopup->IsShown()) {
		event.StopPropagation();		 
	}
	else {
		event.Skip();
	}
}

void TransactionDialog::OnTagsKillFocus(wxFocusEvent& event) {
	tagsPopup->Hide();
	event.Skip();
}

void TransactionDialog::OnSelectTag() {
	AddTag();
	tagsPopup->Hide();	
}

void TransactionDialog::AddTag() {
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