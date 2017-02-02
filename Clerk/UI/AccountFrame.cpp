#include "AccountFrame.h"

AccountFrame::AccountFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

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

	wxFloatingPointValidator<float> validator(2, &amountValue, wxNUM_VAL_DEFAULT);
	validator.SetRange(0.0f, 999999999.0f);

	amountField = new wxTextCtrl(this, wxID_ANY, "0.00", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, validator);
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

	typeList->SetSelection(0);

	for each (auto currency in DataHelper::GetInstance().GetCurrencies())
	{
		currencies.push_back(currency);
		currencyList->AppendString(*currency->name);
	}

	currencyList->SetSelection(191);

	wxImage image;

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);

		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			iconList->Append("", *bitmap);

			delete bitmap;
		}
	}

	iconList->SetSelection(0);

	okButton->Bind(wxEVT_BUTTON, &AccountFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AccountFrame::OnCancel, this);
}

AccountFrame::~AccountFrame() {
	delete nameLabel;
	delete nameField;
	delete typeLabel;
	delete typeList;
	delete currencyLabel;
	delete currencyList;
	delete iconLabel;
	delete iconList;
	delete amountLabel;
	delete amountField;
	delete noteLabel;
	delete noteField;
	delete okButton;
	delete cancelButton;
}

void AccountFrame::SetAccount(std::shared_ptr<Account> account) {
	this->account = account;

	wxString name = wxString::FromUTF8(account->name.get()->c_str());

	nameField->SetValue(name);
	noteField->SetValue(*account->note);
	typeList->SetSelection(static_cast<int>(account->type));

	if (account->iconId < iconList->GetCount()) {
		iconList->SetSelection(static_cast<int>(account->iconId));
	} else {
		iconList->SetSelection(0);
	}

	if (account->id != -1) {
		amountField->Disable();
	} else {
		amountField->Enable();
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
}

void AccountFrame::OnOK(wxCommandEvent &event) {
	double val;

	amountField->GetValue().ToDouble(&val);
	amountValue = val;

	bool isNew = false;

	if (account->id == -1) {
		isNew = true;
	}

	account->name = make_shared<string>(nameField->GetValue());
	account->note = make_shared<string>(noteField->GetValue());
	account->type = static_cast<AccountTypes>(typeList->GetSelection());
	account->iconId = iconList->GetSelection();
	account->currency = currencies[currencyList->GetSelection()];

	account->Save();

	if ((account->type == AccountTypes::Debt || account->type == AccountTypes::Credit) && isNew) {
		Transaction *transaction = new Transaction();

		transaction->fromAccountId = account->id;
		transaction->toAccountId = -1;
		transaction->fromAmount = amountValue;
		transaction->toAmount = amountValue;
		transaction->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

		transaction->Save();
	}

	Close();

	if (OnClose) {
		OnClose();
	}
}

void AccountFrame::OnCancel(wxCommandEvent &event) {
	Close();
}