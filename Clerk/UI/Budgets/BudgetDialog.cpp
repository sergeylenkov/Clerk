#include "BudgetDialog.h"

BudgetDialog::BudgetDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
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

	wxStaticText *nameText = new wxStaticText(mainPanel, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(nameText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *periodText = new wxStaticText(mainPanel, wxID_ANY, wxT("Period:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(periodText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	periodList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(periodList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(datePicker, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *amountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Amount:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	amountField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(amountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 1, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *accountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Accounts:"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(accountLabel, 0, wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

	accountsList = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	verticalSizer->Add(accountsList, 1, wxALL | wxEXPAND, 5);

	//accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

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

	periodList->AppendString("Week");
	periodList->AppendString("Month");
	periodList->AppendString("Year");
	periodList->AppendString("Custom");

	periodList->SetSelection(static_cast<int>(BudgetPeriod::Month));

	UpdateAccounts();

	periodList->Bind(wxEVT_COMBOBOX, &BudgetDialog::OnPeriodSelect, this);

	okButton->Bind(wxEVT_BUTTON, &BudgetDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &BudgetDialog::OnCancel, this);
	Bind(wxEVT_CHAR_HOOK, &BudgetDialog::OnKeyDown, this);
}

void BudgetDialog::SetBudget(std::shared_ptr<BudgetViewModel> budget) {
	_budget = budget;

	nameField->SetValue(budget->name);	
	periodList->SetSelection(static_cast<int>(budget->period));
	amountField->SetValue(wxString::Format("%.2f", budget->amount));
	datePicker->SetValue(budget->date);

	datePicker->Disable();

	if (budget->period == BudgetPeriod::Custom) {
		datePicker->Enable();
	}

	/*std::string str = budget->accountIds->mb_str();
	std::set<int> ids;

	std::stringstream ss(str);

	int i;

	while (ss >> i)
	{
		ids.insert(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	i = 0;

	for (auto& account : accounts)
	{
		bool checked = ids.count(account->id) > 0;
		accountsList->CheckItem(i, checked);

		i++;
	}*/
}

void BudgetDialog::UpdateAccounts() {
	accountsList->ClearAll();
	accountsList->EnableCheckBoxes(true);

	//accounts = DataHelper::GetInstance().GetAccountsByType(Account::Type::Expens);
	//auto debts = DataHelper::GetInstance().GetAccountsByType(Account::Type::Debt);

	//accounts.insert(accounts.end(), debts.begin(), debts.end());

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	accountsList->InsertColumn(0, column);	

	int i = 0;

	for (auto &account : accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		accountsList->InsertItem(listItem);
		/*accountsList->SetItem(i, 0, account->name);

		accountsList->SetItemImage(listItem, account->iconId);*/

		i++;
	}
}

void BudgetDialog::OnPeriodSelect(wxCommandEvent &event) {
	if (periodList->GetSelection() == static_cast<int>(BudgetPeriod::Custom)) {
		datePicker->Enable();
	}
	else {
		datePicker->Disable();
	}
}

void BudgetDialog::OnOK(wxCommandEvent &event) {
	double val;

	amountField->GetValue().ToDouble(&val);
	amountValue = val;

	_budget->name = nameField->GetValue();
	_budget->period = static_cast<BudgetPeriod>(periodList->GetSelection());
	_budget->date = datePicker->GetValue();
	_budget->amount = amountValue;
	
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
			accountIds = accountIds + wxString::Format("%i,", account->id);
		}
	}

	accountIds.RemoveLast();

	//budget->accountIds = std::make_shared<wxString>(accountIds);

	//budget->Save();

	Close();

	if (OnClose) {
		OnClose();
	}
}

void BudgetDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void BudgetDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}