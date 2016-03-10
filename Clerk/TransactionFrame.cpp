#include "TransactionFrame.h"

TransactionFrame::TransactionFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(* wxWHITE));

	wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	panel->SetBackgroundColour(wxColor(*wxWHITE));

	fromLabel = new wxStaticText(panel, wxID_ANY, "From:", wxPoint(10, 10), wxSize(60, 20));
	toLabel = new wxStaticText(panel, wxID_ANY, "To:", wxPoint(10, 40), wxSize(60, 20));

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

	fromList = new wxComboBox(panel, wxID_ANY, "", wxPoint(100, 10), wxSize(200, 20), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	toList = new wxComboBox(panel, wxID_ANY, "", wxPoint(100, 40), wxSize(200, 20), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	
	for each (auto account in accounts) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			fromList->AppendString(*account->name);
			fromAccounts.push_back(account);
		}
	}
	
	fromValue = 0;
	toValue = 0;

	wxFloatingPointValidator<float> fromValidator(2, &fromValue, wxNUM_VAL_DEFAULT);
	fromValidator.SetRange(0.0f, 999999999.0f);
	
	fromAmountField = new wxTextCtrl(panel, wxID_ANY, "0.00", wxPoint(width - 140, 10), wxSize(80, 20), wxTE_RIGHT, fromValidator);
	fromAmountLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(width - 50, 10), wxSize(40, 20));

	wxFloatingPointValidator<float> toValidator(2, &toValue, wxNUM_VAL_DEFAULT);
	toValidator.SetRange(0.0f, 999999999.0f);

	toAmountField = new wxTextCtrl(panel, wxID_ANY, "0.00", wxPoint(width - 140, 40), wxSize(80, 20), wxTE_RIGHT, toValidator);
	toAmountLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(width - 50, 40), wxSize(40, 20));

	dateLabel = new wxStaticText(panel, wxID_ANY, "Date:", wxPoint(10, 80), wxSize(60, 20));
	datePicker = new wxDatePickerCtrl(panel, wxID_ANY, wxDefaultDateTime, wxPoint(100, 80), wxSize(100, 20), wxDP_DROPDOWN);

	tagsLabel = new wxStaticText(panel, wxID_ANY, "Tags:", wxPoint(10, 120), wxSize(60, 20));
	tagsField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 120), wxSize(330, 20));

	noteLabel = new wxStaticText(panel, wxID_ANY, "Note:", wxPoint(10, 150), wxSize(60, 20));
	noteField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 150), wxSize(330, 60), wxTE_MULTILINE);

	okButton = new wxButton(panel, wxID_ANY, "OK", wxPoint(width - 200, height - 70), wxSize(80, 28));
	cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxPoint(width - 100, height - 70), wxSize(80, 28));

	okButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionFrame::OnFromAmountKillFocus, this);

	SelectFromAccount(0);
	SelectToAccount(0);
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
}

void TransactionFrame::SetAccount(shared_ptr<Account> account) {
	if (account->type == AccountTypes::Receipt) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (account->id == accounts[i]->id) {
				SelectFromAccount(i);
				transaction->fromAccountId = account->id;
				break;
			}
		}
	}

	if (account->type == AccountTypes::Deposit) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (account->id == accounts[i]->id) {
				SelectFromAccount(i);
				transaction->fromAccountId = account->id;
				break;
			}
		}
	}

	if (account->type == AccountTypes::Expens) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (account->id == accounts[i]->id) {
				SelectToAccount(i);
				transaction->toAccountId = account->id;
				break;
			}
		}
	}

	if (account->type == AccountTypes::Debt) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (account->id == accounts[i]->id) {
				SelectToAccount(i);
				transaction->toAccountId = account->id;
				break;
			}
		}
	}

	if (account->type == AccountTypes::Credit) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (account->id == accounts[i]->id) {
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

void TransactionFrame::OnOK(wxCommandEvent &event) {
	double val;

	fromAmountField->GetValue().ToDouble(&val);
	fromValue = val;
	
	toAmountField->GetValue().ToDouble(&val);
	toValue = val;

	transaction->fromAccountId = fromAccounts[fromList->GetSelection()]->id;
	transaction->toAccountId = toAccounts[toList->GetSelection()]->id;
	transaction->fromAmount = fromValue;
	transaction->toAmount = toValue;
	transaction->note = make_shared<wxString>(noteField->GetValue());
	transaction->tags = make_shared<wxString>(tagsField->GetValue());
	transaction->paidAt = make_shared<wxDateTime>(datePicker->GetValue());

	transaction->Save();

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
	auto account = fromAccounts[id];
	
	fromList->Select(id);
	fromAmountLabel->SetLabel(*account->currency->shortName);

	toList->Clear();
	toAccounts.clear();

	for each (auto toAccount in accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == AccountTypes::Receipt && toAccount->type == AccountTypes::Deposit) {
			toList->AppendString(*toAccount->name);
			toAccounts.push_back(toAccount);
		}

		if (account->type == AccountTypes::Deposit && (toAccount->type == AccountTypes::Deposit || toAccount->type == AccountTypes::Expens || toAccount->type == AccountTypes::Debt || toAccount->type == AccountTypes::Credit)) {
			toList->AppendString(*toAccount->name);
			toAccounts.push_back(toAccount);
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