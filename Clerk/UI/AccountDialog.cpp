#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxString allowedChars[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " " };
	wxArrayString chars(13, allowedChars);
	wxTextValidator amountValidator(wxFILTER_INCLUDE_CHAR_LIST);
	amountValidator.SetIncludes(chars);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer *fieldsSizer = new wxFlexGridSizer(6, 2, 10, 0);
	fieldsSizer->AddGrowableCol(1);
	fieldsSizer->AddGrowableRow(5);
	fieldsSizer->SetFlexibleDirection(wxBOTH);
	fieldsSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	nameLabel = new wxStaticText(this, wxID_ANY, "Name:", wxDefaultPosition, wxDefaultSize, 0);
	nameLabel->Wrap(-1);
	fieldsSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(nameField, 0, wxALL | wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

	typeLabel = new wxStaticText(this, wxID_ANY, "Type:", wxDefaultPosition, wxDefaultSize, 0);
	typeLabel->Wrap(-1);
	fieldsSizer->Add(typeLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	typeList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(typeList, 0, wxALL | wxEXPAND, 5);

	currencyLabel = new wxStaticText(this, wxID_ANY, "Currency:", wxDefaultPosition, wxDefaultSize, 0);
	currencyLabel->Wrap(-1);
	fieldsSizer->Add(currencyLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	currencyList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(currencyList, 0, wxALL | wxEXPAND, 5);

	iconLabel = new wxStaticText(this, wxID_ANY, "Icon:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(iconLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	iconList = new wxBitmapComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(iconList, 0, wxALL, 5);

	amountLabel = new wxStaticText(this, wxID_ANY, "Amount:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	
	amountField = new wxTextCtrl(this, wxID_ANY, "0.00", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, amountValidator);
	fieldsSizer->Add(amountField, 0, wxALL, 5);

	noteLabel = new wxStaticText(this, wxID_ANY, "Note:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(noteLabel, 0, wxALIGN_TOP | wxALL, 5);

	noteField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	fieldsSizer->Add(noteField, 0, wxALL | wxEXPAND, 5);

	mainSizer->Add(fieldsSizer, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(okButton, 0, wxALIGN_CENTER | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(cancelButton, 0, wxALL, 5);

	mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

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

	for each (auto currency in DataHelper::GetInstance().GetCurrencies())
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
}

AccountDialog::~AccountDialog() {
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

	for each (auto currency in currencies)
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
		if (account->type == AccountTypes::Debt || account->type == AccountTypes::Credit) {
			if (isNew) {
				Transaction *transaction = new Transaction();

				transaction->fromAccountId = account->id;
				transaction->toAccountId = -1;
				transaction->fromAmount = amountValue;
				transaction->toAmount = amountValue;
				transaction->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

				transaction->Save();
			}
			else if (initialTransaction) {
				initialTransaction->fromAmount = amountValue;
				initialTransaction->toAmount = amountValue;

				initialTransaction->Save();
			}
		}
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

wxString AccountDialog::ClearAmountValue(wxString &value) {
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);

	return value;
}