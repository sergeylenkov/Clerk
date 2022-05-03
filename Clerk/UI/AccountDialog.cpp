#include "AccountDialog.h"

AccountDialog::AccountDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height, Icons& icons) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)), _icons(icons) {
	SetBackgroundColour(wxColor(*wxWHITE));
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
	

	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	int index = 0;
	int i = 0;

	//TODO
	/*for (auto currency : DataHelper::GetInstance().GetCurrencies())
	{
		currencies.push_back(currency);

		wxString name = wxString::Format("%s (%s)", currency->shortName->c_str(), currency->name->c_str());
		currencyList->AppendString(name);

		if (baseCurrencyId == currency->id) {
			index = i;
		}

		i++;
	}

	currencyList->SetSelection(index);*/
	
	wxImage image;

	/*for (int i = 0; i < DataHelper::GetInstance().accountsImageList->GetImageCount(); i++) {
		iconList->Append("", DataHelper::GetInstance().accountsImageList->GetIcon(i));
	}*/

	iconList->SetSelection(0);

	amountField->Bind(wxEVT_KILL_FOCUS, &AccountDialog::OnAmountKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &AccountDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AccountDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &AccountDialog::OnKeyDown, this);
}

AccountDialog::~AccountDialog() {
	delete _viewModel;
}

void AccountDialog::SetViewModel(AccountEditViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [=]() {
		Update();
	};

	Update();

	nameField->SetFocus();
}

void AccountDialog::Update() {
	auto types = _viewModel->GetTypes();	

	typeList->Clear();

	for (auto& type : types) {
		typeList->AppendString(type);
	}

	typeList->SetSelection(static_cast<int>(_viewModel->GetType()));

	currencyList->Clear();

	auto currencies = _viewModel->GetCurrencies();

	for (auto& currency : currencies) {
		wxString name = wxString::Format("%s (%s)", currency->shortName->c_str(), currency->name->c_str());
		currencyList->AppendString(name);
	}

	currencyList->SetSelection(_viewModel->GetCurrencyIndex());

	nameField->SetValue(_viewModel->GetName());
	noteField->SetValue(_viewModel->GetNote());
}


void AccountDialog::OnOK(wxCommandEvent &event) {
	double amountValue;
	bool isNew = false;

	if (account->id == -1) {
		isNew = true;
	}

	account->name = nameField->GetValue();
	account->note = noteField->GetValue();
	account->type = static_cast<AccountType>(typeList->GetSelection());
	account->icon = iconList->GetSelection();
	account->currency = currencies[currencyList->GetSelection()];

	//TODO moved method to interactor
	//account->Save();
	
	if (amountValue > 0) {		
		if (isNew) {
			if (account->type == AccountType::Debt) {
				/*TransactionViewModel* transaction = new TransactionViewModel();

				transaction->fromAccount = account;				
				transaction->fromAmount = amountValue;
				transaction->toAmount = amountValue;
				transaction->date = make_shared<wxDateTime>(wxDateTime::Now());*/

				//transaction->Save();
			}
			else if (account->type == AccountType::Deposit || account->type == AccountType::Virtual) {
				/*Transaction *transaction = new Transaction();

				transaction->toAccount = account;
				transaction->fromAmount = amountValue;
				transaction->toAmount = amountValue;
				transaction->date = wxDateTime::Now();*/

				//transaction->Save();
			}
		}
		else if (initialTransaction) {
			initialTransaction->fromAmount = amountValue;
			initialTransaction->toAmount = amountValue;

			//initialTransaction->Save();
		}		
	}

	//TODO
	if (isNew) {
		//DataHelper::GetInstance().ReloadAccounts();
	}

	Close();
}

void AccountDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void AccountDialog::OnAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	_viewModel->SetAmount(amountField->GetFloatValue());
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