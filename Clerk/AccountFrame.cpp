#include "AccountFrame.h"

AccountFrame::AccountFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	panel->SetBackgroundColour(wxColor(*wxWHITE));

	nameLabel = new wxStaticText(panel, wxID_ANY, "Name:", wxPoint(10, 10), wxSize(60, 20));
	nameField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 10), wxSize(180, 20));

	typeLabel = new wxStaticText(panel, wxID_ANY, "Type:", wxPoint(10, 50), wxSize(60, 20));
	typeList = new wxComboBox(panel, wxID_ANY, "", wxPoint(100, 50), wxSize(180, 20), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);

	typeList->AppendString("Receipt");
	typeList->AppendString("Deposit");
	typeList->AppendString("Expens");
	typeList->AppendString("Debt");
	
	typeList->SetSelection(0);

	currencyLabel = new wxStaticText(panel, wxID_ANY, "Currency:", wxPoint(10, 90), wxSize(60, 20));
	currencyList = new wxComboBox(panel, wxID_ANY, "", wxPoint(100, 90), wxSize(180, 20), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);

	for each (auto currency in DataHelper::GetInstance().GetCurrencies())
	{
		currencies.push_back(currency);
		currencyList->AppendString(*currency->name);
	}

	currencyList->SetSelection(191);

	wxFloatingPointValidator<float> validator(2, &amountValue, wxNUM_VAL_DEFAULT);
	validator.SetRange(0.0f, 999999999.0f);

	amountLabel = new wxStaticText(panel, wxID_ANY, "Amount:", wxPoint(10, 130), wxSize(60, 20));
	amountField = new wxTextCtrl(panel, wxID_ANY, "0.00", wxPoint(100, 130), wxSize(80, 20), wxTE_RIGHT, validator);

	noteLabel = new wxStaticText(panel, wxID_ANY, "Note:", wxPoint(10, 170), wxSize(60, 20));
	noteField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 170), wxSize(180, 60), wxTE_MULTILINE);

	okButton = new wxButton(panel, wxID_ANY, "OK", wxPoint(width - 200, height - 70), wxSize(80, 28));
	cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxPoint(width - 100, height - 70), wxSize(80, 28));

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
	delete amountLabel;
	delete amountField;
	delete noteLabel;
	delete noteField;
	delete okButton;
	delete cancelButton;
}

void AccountFrame::SetAccount(std::shared_ptr<Account> account) {
	this->account = account;

	nameField->SetValue(*account->name);
	noteField->SetValue(*account->note);
	typeList->SetSelection(static_cast<int>(account->type));

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

	account->name = make_shared<wxString>(nameField->GetValue());
	account->note = make_shared<wxString>(noteField->GetValue());
	account->type = static_cast<AccountTypes>(typeList->GetSelection());
	account->currency = currencies[currencyList->GetSelection()];

	if (account->id != -1) {
		DataHelper::GetInstance().UpdateAccount(account.get());
	} else {
		DataHelper::GetInstance().AddAccount(account.get());
	}

	Close();

	if (OnClose) {
		OnClose();
	}
}

void AccountFrame::OnCancel(wxCommandEvent &event) {
	Close();
}