#include "TransactionFrame.h"

TransactionFrame::TransactionFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(* wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxFloatingPointValidator<float> fromValidator(2, &fromValue, wxNUM_VAL_DEFAULT);
	fromValidator.SetRange(0.0f, 999999999.0f);

	wxFloatingPointValidator<float> toValidator(2, &toValue, wxNUM_VAL_DEFAULT);
	toValidator.SetRange(0.0f, 999999999.0f);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer *fieldsSizer = new wxFlexGridSizer(5, 2, 10, 0);
	fieldsSizer->AddGrowableCol(1);
	fieldsSizer->AddGrowableRow(4);
	fieldsSizer->SetFlexibleDirection(wxBOTH);
	fieldsSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	fromLabel = new wxStaticText(this, wxID_ANY, "From:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	wxPanel *fromPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *fromSizer = new wxBoxSizer(wxHORIZONTAL);

	fromList = new wxBitmapComboBox(fromPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	fromSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(fromPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, fromValidator);
	fromSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(fromPanel, wxID_ANY, "RUB", wxDefaultPosition, wxDefaultSize, 0);
	fromSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromPanel->SetSizer(fromSizer);
	fromPanel->Layout();

	fromSizer->Fit(fromPanel);

	fieldsSizer->Add(fromPanel, 1, wxEXPAND | wxALL, 5);

	toLabel = new wxStaticText(this, wxID_ANY, "To:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	wxPanel *toPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *toSizer = new wxBoxSizer(wxHORIZONTAL);

	toList = new wxBitmapComboBox(toPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	toSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(toPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, toValidator);
	toSizer->Add(toAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountLabel = new wxStaticText(toPanel, wxID_ANY, "RUB", wxDefaultPosition, wxDefaultSize, 0);
	toSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toPanel->SetSizer(toSizer);
	toPanel->Layout();

	toSizer->Fit(toPanel);

	fieldsSizer->Add(toPanel, 1, wxEXPAND | wxALL, 5);

	dateLabel = new wxStaticText(this, wxID_ANY, "Date:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	datePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	fieldsSizer->Add(datePicker, 0, wxALL, 5);

	tagsLabel = new wxStaticText(this, wxID_ANY, "Tags:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(tagsField, 0, wxALL | wxEXPAND, 5);

	noteLabel = new wxStaticText(this, wxID_ANY, "Note:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(noteLabel, 0, wxALL, 5);

	noteField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	fieldsSizer->Add(noteField, 0, wxALL | wxEXPAND, 5);

	mainSizer->Add(fieldsSizer, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, "OK", wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 5);

	mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	okButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionFrame::OnFromAmountKillFocus, this);
	fromAmountField->Bind(wxEVT_KEY_DOWN, &TransactionFrame::OnTextChanged, this);

	fromValue = 0;
	toValue = 0;

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

	accountsImageList = new wxImageList(16, 16, true);
	wxImage image;

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	for each (auto account in accounts) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			//fromList->AppendString(*account->name);
			int icon = 0;

			if (account->iconId < accountsImageList->GetImageCount()) {
				icon = account->iconId;
			}

			fromList->Append(*account->name, accountsImageList->GetBitmap(icon));

			fromAccounts.push_back(account);
		}
	}

	SelectFromAccount(0);
}

TransactionFrame::~TransactionFrame() {
	delete fromLabel;
	delete toLabel;
	delete fromList;
	delete toList;
	delete fromAmountField;
	delete fromAmountLabel;
	delete okButton;
	delete cancelButton;
	delete tagsLabel;
	delete tagsField;
	delete noteLabel;
	delete noteField;
	delete dateLabel;
	delete datePicker;
	delete accountsImageList;
}

void TransactionFrame::SetAccount(shared_ptr<Account> account) {
	if (account->type == AccountTypes::Receipt) {
		for (unsigned int i = 0; i < fromAccounts.size(); i++) {
			if (account->id == fromAccounts[i]->id) {
				SelectFromAccount(i);
				transaction->fromAccountId = account->id;
				break;
			}
		}

		int id = DataHelper::GetInstance().GetPairAccountId(account.get());

		if (id == -1) {
			for (unsigned int i = 0; i < toAccounts.size(); i++) {
				if (toAccounts[i]->type == AccountTypes::Deposit) {
					SelectToAccount(i);
					break;
				}
			}
		} else {
			for (unsigned int i = 0; i < toAccounts.size(); i++) {
				if (toAccounts[i]->id == id) {
					SelectToAccount(i);
					break;
				}
			}
		}
	} else if (account->type == AccountTypes::Deposit) {
		for (unsigned int i = 0; i < fromAccounts.size(); i++) {
			if (account->id == fromAccounts[i]->id) {
				SelectFromAccount(i);
				transaction->fromAccountId = account->id;
				break;
			}
		}
	} else if (account->type == AccountTypes::Expens) {
		int id = DataHelper::GetInstance().GetPairAccountId(account.get());

		if (id == -1) {
			for (unsigned int i = 0; i < fromAccounts.size(); i++) {
				if (fromAccounts[i]->type == AccountTypes::Deposit) {
					SelectFromAccount(i);
					break;
				}
			}
		} else {
			for (unsigned int i = 0; i < fromAccounts.size(); i++) {
				if (fromAccounts[i]->id == id) {
					SelectFromAccount(i);
					break;
				}
			}
		}		

		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (account->id == toAccounts[i]->id) {
				SelectToAccount(i);
				transaction->toAccountId = account->id;
				break;
			}
		}
	} else if (account->type == AccountTypes::Debt) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (account->id == toAccounts[i]->id) {
				SelectToAccount(i);
				transaction->toAccountId = account->id;
				break;
			}
		}
	} else if (account->type == AccountTypes::Credit) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (account->id == toAccounts[i]->id) {
				SelectToAccount(i);
				transaction->toAccountId = account->id;
				break;
			}
		}
	}
}

void TransactionFrame::SetTransaction(std::shared_ptr<Transaction> transaction) {
	this->transaction = transaction;

	fromAmountField->SetValue(wxString::Format("%.2f", transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", transaction->toAmount));
	tagsField->SetValue(*transaction->tags);
	noteField->SetValue(*transaction->note);
	datePicker->SetValue(*transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			break;
		}
	}

	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (transaction->toAccountId == toAccounts[i]->id) {
			SelectToAccount(i);
			break;
		}
	}
}

void TransactionFrame::SetSplitTransaction(std::shared_ptr<Transaction> transaction) {
	auto copy = make_shared<Transaction>();

	copy->fromAccountId = transaction->fromAccountId;
	copy->toAccountId = transaction->toAccountId;
	copy->fromAmount = transaction->fromAmount;
	copy->toAmount = transaction->toAmount;
	copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

	this->transaction = copy;
	this->splitTransaction = transaction;

	fromAmountField->SetValue(wxString::Format("%.2f", transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", transaction->toAmount));
	tagsField->SetValue("");
	noteField->SetValue("");
	datePicker->SetValue(*transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			break;
		}
	}

	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (transaction->toAccountId == toAccounts[i]->id) {
			SelectToAccount(i);
			break;
		}
	}
}

void TransactionFrame::OnOK(wxCommandEvent &event) {
	double val;

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
	transaction->paidAt = make_shared<wxDateTime>(datePicker->GetValue());

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

void TransactionFrame::OnCancel(wxCommandEvent &event) {
	Close();
}

void TransactionFrame::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());
}

void TransactionFrame::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());
}

void TransactionFrame::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	double val;

	toAmountField->GetValue().ToDouble(&val);

	if (val == 0) {
		toAmountField->SetValue(fromAmountField->GetValue());
	}
}

void TransactionFrame::SelectFromAccount(int id) {
	int index = fromList->GetSelection();
	auto account = fromAccounts[id];
	
	fromList->Select(id);
	fromAmountLabel->SetLabel(*account->currency->shortName);

	if (index != -1) {
		if (account->type == fromAccounts[index]->type) {
			return;
		}
	}

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
		} else if (account->type == AccountTypes::Deposit) {
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

	toList->Select(0);
	SelectToAccount(0);
}

void TransactionFrame::SelectToAccount(int id) {
	auto account = toAccounts[id];
	
	toList->Select(id);
	toAmountLabel->SetLabel(*account->currency->shortName);
}

void TransactionFrame::OnTextChanged(wxKeyEvent &event) {
	wxLogDebug("key press %s", fromAmountField->GetValue().c_str());
	event.Skip();
}