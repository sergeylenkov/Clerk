#include "BudgetDialog.h"

BudgetDialog::BudgetDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetIcon(wxICON(APP_ICON));

	wxString allowedChars[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " " };
	wxArrayString chars(13, allowedChars);
	wxTextValidator amountValidator(wxFILTER_INCLUDE_CHAR_LIST);
	amountValidator.SetIncludes(chars);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *nameText = new wxStaticText(this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(50, -1), 0);	
	horizontalSizer->Add(nameText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *periodText = new wxStaticText(this, wxID_ANY, wxT("Period:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(periodText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	periodList = new wxBitmapComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(periodList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	datePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(datePicker, 0, wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *amountLabel = new wxStaticText(this, wxID_ANY, wxT("Amount:"), wxDefaultPosition, wxSize(50, -1), 0);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	amountField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(amountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 1, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *accountLabel = new wxStaticText(this, wxID_ANY, wxT("Accounts:"), wxDefaultPosition, wxDefaultSize, 0);	
	horizontalSizer->Add(accountLabel, 0, wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

	accountsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	verticalSizer->Add(accountsList, 1, wxALL | wxEXPAND, 5);

	accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

	mainSizer->Add(verticalSizer, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	periodList->AppendString("Week");
	periodList->AppendString("Month");
	periodList->AppendString("Year");
	periodList->AppendString("Custom");

	periodList->SetSelection(static_cast<int>(BudgetPeriods::Month));

	UpdateAccounts();

	periodList->Bind(wxEVT_COMBOBOX, &BudgetDialog::OnPeriodSelect, this);

	okButton->Bind(wxEVT_BUTTON, &BudgetDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &BudgetDialog::OnCancel, this);
}

BudgetDialog::~BudgetDialog() {
	
}

void BudgetDialog::SetBudget(std::shared_ptr<Budget> budget) {
	this->budget = budget;

	nameField->SetValue(*this->budget->name);	
	periodList->SetSelection(static_cast<int>(this->budget->period));
	amountField->SetValue(wxString::Format("%.2f", this->budget->amount));
	datePicker->SetValue(*budget->date);

	datePicker->Disable();

	if (budget->period == BudgetPeriods::Custom) {
		datePicker->Enable();
	}

	std::string str = budget->accountIds->mb_str();
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

	i = 0;

	for each (auto account in accounts)
	{
		if (std::find(ids.begin(), ids.end(), account->id) != ids.end()) {
			accountsList->CheckItem(i, true);
		}

		i++;
	}
}

void BudgetDialog::UpdateAccounts() {
	accountsList->ClearAll();
	accountsList->EnableCheckboxes(true);

	accounts = DataHelper::GetInstance().GetAccountsByType(AccountTypes::Expens);

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	accountsList->InsertColumn(0, column);	

	int i = 0;

	for each (auto account in accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		accountsList->InsertItem(listItem);
		accountsList->SetItem(i, 0, *account->name);

		accountsList->SetItemImage(listItem, account->iconId);

		i++;
	}
}

void BudgetDialog::OnPeriodSelect(wxCommandEvent &event) {
	if (periodList->GetSelection() == static_cast<int>(BudgetPeriods::Custom)) {
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

	budget->name = make_shared<wxString>(nameField->GetValue());
	budget->period = static_cast<BudgetPeriods>(periodList->GetSelection());
	budget->date = make_shared<wxDateTime>(datePicker->GetValue());
	budget->amount = amountValue;
	
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

	budget->accountIds = make_shared<wxString>(accountIds);

	budget->Save();

	Close();

	if (OnClose) {
		OnClose();
	}
}

void BudgetDialog::OnCancel(wxCommandEvent &event) {
	Close();
}