#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	transactionsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	transactionsList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &TransactionsListPanel::OnListItemClick, this);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(this, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("This Week"));
	values->Add(wxT("Previous Week"));
	values->Add(wxT("This Month"));
	values->Add(wxT("This Year"));
	values->Add(wxT("Custom"));

	periodList = new wxComboBox(this, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	periodList->Bind(wxEVT_COMBOBOX, &TransactionsListPanel::OnPeriodSelect, this);

	wxStaticText *st1 = new wxStaticText(this, wxID_ANY, wxT("From:"));
	fromDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);
	wxStaticText *st2 = new wxStaticText(this, wxID_ANY, wxT("To:"));
	toDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	periodList->Select(2);

	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);

	topsizer->Add(st3, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	topsizer->Add(periodList, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	topsizer->Add(st1, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	topsizer->Add(fromDatePicker, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	topsizer->Add(st2, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	topsizer->Add(toDatePicker, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 0);

	sizer->Add(topsizer, 0, wxTOP | wxBOTTOM, 10);
	sizer->Add(transactionsList, 1, wxEXPAND);

	this->SetSizer(sizer);
	this->Layout();

	CalculatePeriod();
}

TransactionsListPanel::~TransactionsListPanel() {
	delete periodList;
	delete fromDatePicker;
	delete toDatePicker;
}

void TransactionsListPanel::SetAccount(Account *account) {
	this->account = account;
}

Transaction *TransactionsListPanel::GetTransaction() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = transactionsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return DataHelper::GetInstance().GetTransaction((int)transactionsList->GetItemData(itemIndex));
	}

	return NULL;
}

void TransactionsListPanel::Update() {
	transactionsList->ClearAll();

	if (account == NULL) {
		return;
	}

	wxListItem col0;

	col0.SetId(0);
	col0.SetText(_("Account"));
	col0.SetWidth(200);

	transactionsList->InsertColumn(0, col0);

	wxListItem col1;

	col1.SetId(1);
	col1.SetText(_("Tags"));
	col1.SetWidth(200);

	transactionsList->InsertColumn(1, col1);

	wxListItem col2;

	col2.SetId(2);
	col2.SetText(_("Date"));
	col2.SetWidth(100);

	transactionsList->InsertColumn(2, col2);

	wxListItem col3;

	col3.SetId(3);
	col3.SetText(_("Amount"));
	col3.SetWidth(100);
	col3.SetAlign(wxLIST_FORMAT_RIGHT);

	transactionsList->InsertColumn(3, col3);

	int i = 0;
	wxDateTime date = wxDateTime::Now() - wxTimeSpan::Days(30);
	
	for each (auto transaction in DataHelper::GetInstance().GetTransactions(account, &fromDatePicker->GetValue(), &toDatePicker->GetValue()))
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(transaction->id);

		transactionsList->InsertItem(listItem);

		if (account->id == transaction->from_account_id) {
			transactionsList->SetItem(i, 0, *transaction->to_account_name);
			transactionsList->SetItemTextColour(i, wxColor(255, 0, 0, 1));
		}

		if (account->id == transaction->to_account_id) {
			transactionsList->SetItem(i, 0, *transaction->from_account_name);
			transactionsList->SetItemTextColour(i, wxColor(0, 0, 0, 1));
		}

		transactionsList->SetItemImage(i, 1);
		transactionsList->SetItem(i, 1, *transaction->tags);
		transactionsList->SetItem(i, 2, transaction->paid_at->Format("%B %d"));

		if (account->id == transaction->from_account_id) {
			transactionsList->SetItem(i, 3, wxString::Format("%.2f", transaction->from_amount));
		}
		else if (account->id == transaction->to_account_id) {
			transactionsList->SetItem(i, 3, wxString::Format("%.2f", transaction->to_amount));
		}
		else {
			transactionsList->SetItem(i, 3, wxString::Format("%.2f", transaction->from_amount));
		}

		i++;
	}
}

void TransactionsListPanel::EditTransaction() {
	if (OnEditTransaction) {
		OnEditTransaction();
	}
}

void TransactionsListPanel::DeleteTransaction() {
	Transaction *transaction = GetTransaction();

	if (transaction) {
		DataHelper::GetInstance().DeleteTransaction(transaction->id);
		Update();
	}
}

void TransactionsListPanel::DublicateTransaction() {
	Transaction *transaction = GetTransaction();

	if (transaction) {
		Transaction *copy = new Transaction();

		copy->id = -1;
		copy->from_account_id = transaction->from_account_id;
		copy->to_account_id = transaction->to_account_id;
		copy->from_amount = transaction->from_amount;
		copy->to_amount = transaction->to_amount;
		copy->note = transaction->note;
		copy->tags = transaction->tags;
		copy->paid_at = make_shared<wxDateTime>(wxDateTime::Now());

		DataHelper::GetInstance().AddTransaction(copy);

		delete copy;

		Update();
	}
}

void TransactionsListPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(ID_EditTransaction, wxT("Edit..."));
	menu->Append(ID_DublicateTransaction, wxT("Dublicate"));
	menu->AppendSeparator();
	menu->Append(ID_DeleteTransaction, wxT("Delete..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionsListPanel::OnMenuSelect, this);

	transactionsList->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void TransactionsListPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (event.GetId()) {
		case ID_AddTransaction:
			//AddTransaction();
			break;

		case ID_EditTransaction:
			EditTransaction();
			break;

		case ID_DeleteTransaction:
			DeleteTransaction();
			break;

		case ID_DublicateTransaction:
			DublicateTransaction();
			break;

		default:
			break;
	}
}

void TransactionsListPanel::OnPeriodSelect(wxCommandEvent &event) {
	CalculatePeriod();
	
	if (OnPeriodChanged) {
		OnPeriodChanged();
	}
}

void TransactionsListPanel::OnDateChanged(wxDateEvent &event) {
	if (OnPeriodChanged) {
		OnPeriodChanged();
	}
}

wxDateTime TransactionsListPanel::GetFromDate() {
	return fromDatePicker->GetValue();
}

wxDateTime TransactionsListPanel::GetToDate() {
	return toDatePicker->GetValue();
}

void TransactionsListPanel::CalculatePeriod() {
	int index = periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDatePicker->Disable();
	toDatePicker->Disable();

	switch (index)
	{
	case 0:
		fromDate.SetToWeekDay(wxDateTime::WeekDay::Mon);
		toDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Sun);
		break;

	case 1:
		fromDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun).SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);
		toDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun);
		break;

	case 2:
		fromDate.SetDay(1);
		toDate.SetToLastMonthDay();
		break;

	case 3:
		fromDate.SetMonth(wxDateTime::Month::Jan);
		fromDate.SetDay(1);
		toDate.SetMonth(wxDateTime::Month::Dec);
		toDate.SetDay(31);
		break;

	case 4:
		fromDatePicker->Enable();
		toDatePicker->Enable();
		break;

	default:
		break;
	}

	

	fromDatePicker->SetValue(fromDate);
	toDatePicker->SetValue(toDate);
}