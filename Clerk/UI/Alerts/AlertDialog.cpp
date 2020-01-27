#include "AlertDialog.h"

AlertDialog::AlertDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
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

	wxStaticText *nameText = new wxStaticText(mainPanel, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(nameText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);	

	typeLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(typeLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	typeList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(90, -1), 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(typeList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	periodLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Period:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	periodList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(90, -1), 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(periodList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	conditionLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Condition:"), wxDefaultPosition, wxSize(60, -1), 0);
	horizontalSizer->Add(conditionLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	conditionList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(90, -1), 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(conditionList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	wxStaticText *amountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Amount:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	amountField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(90, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(amountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 1, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *accountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Accounts:"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(accountLabel, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

	accountsList = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	verticalSizer->Add(accountsList, 1, wxALL | wxEXPAND, 5);

	accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

	panelSizer->Add(verticalSizer, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(mainPanel, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	cancelButton = new wxButton(mainPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	typeList->AppendString("Receipt");
	typeList->AppendString("Expense");
	typeList->AppendString("Balance");

	typeList->SetSelection(static_cast<int>(Alert::Type::Receipt));

	periodList->AppendString("Week");
	periodList->AppendString("Month");
	periodList->AppendString("Year");

	periodList->SetSelection(static_cast<int>(Alert::Period::Month));

	conditionList->AppendString("Less");
	conditionList->AppendString("More");
	conditionList->AppendString("Equal");

	conditionList->SetSelection(static_cast<int>(Alert::Condition::Less));

	typeList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnTypeSelect, this);
	periodList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnPeriodSelect, this);

	okButton->Bind(wxEVT_BUTTON, &AlertDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AlertDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &AlertDialog::OnKeyDown, this);
}

void AlertDialog::SetAlert(std::shared_ptr<Alert> alert) {
	this->alert = alert;

	nameField->SetValue(*this->alert->name);
	typeList->SetSelection(static_cast<int>(this->alert->type));
	periodList->SetSelection(static_cast<int>(this->alert->period));
	amountField->SetValue(wxString::Format("%.2f", this->alert->amount));	

	UpdateAccounts();
}

void AlertDialog::UpdateAccounts() {
	if (typeList->GetSelection() == static_cast<int>(Alert::Type::Expense)) {
		accounts = DataHelper::GetInstance().GetAccountsByType(AccountType::Expens);
		auto debts = DataHelper::GetInstance().GetAccountsByType(AccountType::Debt);

		accounts.insert(accounts.end(), debts.begin(), debts.end());
	}
	
	if (typeList->GetSelection() == static_cast<int>(Alert::Type::Receipt)) {
		accounts = DataHelper::GetInstance().GetAccountsByType(AccountType::Receipt);
	}

	if (typeList->GetSelection() == static_cast<int>(Alert::Type::Balance)) {
		accounts = DataHelper::GetInstance().GetAccountsByType(AccountType::Deposit);
		auto temp = DataHelper::GetInstance().GetAccountsByType(AccountType::Virtual);
		auto debts = DataHelper::GetInstance().GetAccountsByType(AccountType::Debt);

		accounts.insert(accounts.end(), temp.begin(), temp.end());
		accounts.insert(accounts.end(), debts.begin(), debts.end());
	}		

	std::string str = alert->accountIds->mb_str();
	std::vector<int> ids;

	std::stringstream ss(str);

	int i;

	while (ss >> i)
	{
		ids.push_back(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	accountsList->ClearAll();
	accountsList->EnableCheckBoxes(true);

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	accountsList->InsertColumn(0, column);	

	i = 0;

	for (auto &account : accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		accountsList->InsertItem(listItem);
		accountsList->SetItem(i, 0, *account->name);

		accountsList->SetItemImage(listItem, account->iconId);

		if (std::find(ids.begin(), ids.end(), account->id) != ids.end()) {
			accountsList->CheckItem(i, true);
		}

		i++;
	}
}

void AlertDialog::OnTypeSelect(wxCommandEvent &event) {
	UpdateAccounts();
}

void AlertDialog::OnPeriodSelect(wxCommandEvent &event) {
	
}

void AlertDialog::OnOK(wxCommandEvent &event) {
	double val;

	amountField->GetValue().ToDouble(&val);
	amountValue = val;

	alert->name = make_shared<wxString>(nameField->GetValue());
	alert->type = static_cast<Alert::Type>(typeList->GetSelection());
	alert->period = static_cast<Alert::Period>(periodList->GetSelection());	
	alert->condition = static_cast<Alert::Condition>(conditionList->GetSelection());
	alert->amount = amountValue;
	
	wxString accountIds("");

	long itemIndex = -1;

	for (;;) {
		itemIndex = accountsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);

		if (itemIndex == -1) {
			break;
		}

		bool checked = accountsList->IsItemChecked(itemIndex);

		if (checked) {
			auto account = accounts[itemIndex];
			accountIds = accountIds + wxString::Format(wxT("%i,"), account->id);
		}
	}

	accountIds.RemoveLast();

	alert->accountIds = make_shared<wxString>(accountIds);

	alert->Save();

	Close();

	if (OnClose) {
		OnClose();
	}
}

void AlertDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void AlertDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}