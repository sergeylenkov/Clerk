#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));

	wxString allowedChars[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " " };
	wxArrayString chars(13, allowedChars);
	wxTextValidator amountValidator(wxFILTER_INCLUDE_CHAR_LIST);
	amountValidator.SetIncludes(chars);
	
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
	
	amountField = new wxTextCtrl(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
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
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(mainPanel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	typeList->AppendString("Receipt");
	typeList->AppendString("Deposit");
	typeList->AppendString("Expens");
	typeList->AppendString("Debt");
	typeList->AppendString("Credit");
	typeList->AppendString("Virtual");

	typeList->SetSelection(0);

	for (auto currency : DataHelper::GetInstance().GetCurrencies())
	{
		currencies.push_back(currency);

		wxString name = wxString::Format("%s (%s)", currency->shortName->c_str(), currency->name->c_str());
		currencyList->AppendString(name);
	}

	currencyList->SetSelection(191);
	
	wxImage image;

	for (int i = 0; i < DataHelper::GetInstance().accountsImageList->GetImageCount(); i++) {
		iconList->Append("", DataHelper::GetInstance().accountsImageList->GetIcon(i));
	}

	iconList->SetSelection(0);

	amountField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnAmountKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &AccountDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AccountDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &AccountDialog::OnKeyDown, this);
}

void AccountDialog::SetAccount(std::shared_ptr<Account> account) {
	this->account = account;

	nameField->SetValue(*account->name);
	noteField->SetValue(*account->note);
	typeList->SetSelection(static_cast<int>(account->type));

	if (account->iconId < iconList->GetCount()) {
		iconList->SetSelection(account->iconId);
	} else {
		iconList->SetSelection(0);
	}

	int i = 0;

	for (auto currency : currencies)
	{
		if (currency->id == account->currency->id) {
			currencyList->SetSelection(i);
			break;
		}

		i++;
	}

	initialTransaction = DataHelper::GetInstance().GetInitialTransactionForAccount(account.get());

	if (initialTransaction) {
		amountField->SetValue(wxString::Format("%.2f", initialTransaction->fromAmount));
	}
	else {
		amountField->SetValue("0.00");
	}
}

void AccountDialog::OnOK(wxCommandEvent &event) {
	double amountValue;

	wxString value = amountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&amountValue);

	bool isNew = false;

	if (account->id == -1) {
		isNew = true;
	}

	account->name = make_shared<wxString>(nameField->GetValue());
	account->note = make_shared<wxString>(noteField->GetValue());
	account->type = static_cast<AccountTypes>(typeList->GetSelection());
	account->iconId = iconList->GetSelection();
	account->currency = currencies[currencyList->GetSelection()];

	account->Save();
	
	if (amountValue > 0) {		
		if (isNew) {
			if (account->type == AccountTypes::Debt || account->type == AccountTypes::Credit) {
				Transaction *transaction = new Transaction();

				transaction->fromAccountId = account->id;
				transaction->toAccountId = -1;
				transaction->fromAmount = amountValue;
				transaction->toAmount = amountValue;
				transaction->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

				transaction->Save();
			}
			else if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Virtual) {
				Transaction *transaction = new Transaction();

				transaction->fromAccountId = -1;
				transaction->toAccountId = account->id;
				transaction->fromAmount = amountValue;
				transaction->toAmount = amountValue;
				transaction->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

				transaction->Save();
			}
		}
		else if (initialTransaction) {
			initialTransaction->fromAmount = amountValue;
			initialTransaction->toAmount = amountValue;

			initialTransaction->Save();
		}		
	}

	if (isNew) {
		DataHelper::GetInstance().ReloadAccounts();
	}

	Close();

	if (OnClose) {
		OnClose();
	}
}

void AccountDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void AccountDialog::OnAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(amountField->GetValue());
	amountField->SetValue(stringAmount);
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

wxString AccountDialog::ClearAmountValue(wxString &value) {
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);

	return value;
}