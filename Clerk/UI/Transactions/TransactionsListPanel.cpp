#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxBORDER_NONE);

	model = new TransactionsListDataModel();
	list->AssociateModel(model.get());

	//transactionsList->Bind(wxEVT_LIST_COL_CLICK, &TransactionsListPanel::OnListColumnClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TransactionsListPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TransactionsListPanel::OnRightClick, this);
	//transactionsList->Bind(wxEVT_LIST_COL_END_DRAG, &TransactionsListPanel::OnColumnDragged, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *periodSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("Previous Week"));
	values->Add(wxT("This Week"));
	values->Add(wxT("Previous Month"));
	values->Add(wxT("This Month"));
	values->Add(wxT("This Year"));
	values->Add(wxT("Custom"));

	periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	wxStaticText *st1 = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);
	wxStaticText *st2 = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	periodSizer->Add(st3, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(periodList, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(st1, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(st2, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	filterSizer->Add(periodSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	wxBoxSizer *searchSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("Search:"));
	searchField = new wxTextCtrl(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(200, 20));
	searchField->Bind(wxEVT_TEXT, &TransactionsListPanel::OnSearchChanged, this);

	searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	searchSizer->Add(searchField, 0, wxALIGN_CENTER_VERTICAL, 5);

	filterSizer->Add(searchSizer, 0, wxALL, 5);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	infoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));	
	wxBoxSizer *infoSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st4 = new wxStaticText(infoPanel, wxID_ANY, wxT("Transactions:"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(st4, 0, wxALL, 5);

	transactionLabel = new wxStaticText(infoPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(transactionLabel, 0, wxALL, 5);

	wxStaticText *st5 = new wxStaticText(infoPanel, wxID_ANY, wxT("Income:"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(st5, 0, wxALL, 5);

	incomeLabel = new wxStaticText(infoPanel, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(incomeLabel, 0, wxALL, 5);

	wxStaticText *st6 = new wxStaticText(infoPanel, wxID_ANY, wxT("Outcome:"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(st6, 0, wxALL, 5);

	outcomeLabel = new wxStaticText(infoPanel, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
	infoSizer->Add(outcomeLabel, 0, wxALL, 5);

	infoPanel->SetSizer(infoSizer);
	infoPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(infoPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	periodList->Bind(wxEVT_COMBOBOX, &TransactionsListPanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);

	periodList->Select(3);

	CalculatePeriod();
	CreateListColumns();

	balance = 0.0;
	sortBy = 2;
	sortDesc = false;
}

TransactionsListPanel::~TransactionsListPanel() {
	SaveFilterSettings();
	SaveColumnsSettings();	
}

void TransactionsListPanel::SetAccount(shared_ptr<Account> account) {
	this->account = account;
}

shared_ptr<Account> TransactionsListPanel::GetAccount() {
	return this->account;
}

void TransactionsListPanel::SetType(TreeMenuItemTypes type) {
	this->type = type;
}

shared_ptr<Transaction> TransactionsListPanel::GetTransaction() {
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = (int)item.GetID() - 1;
		return filtered[index];
	}

	return nullptr;
}

void TransactionsListPanel::Update() {
	std::thread([=]()
	{
		RestoreFilterSettings();

		if (this->type == TreeMenuItemTypes::MenuAccount) {
			transactions = DataHelper::GetInstance().GetTransactions(*account, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::MenuExpenses) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountTypes::Expens, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::MenuReceipts) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountTypes::Receipt, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::MenuDeposits) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountTypes::Deposit, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::MenuAccounts) {
			transactions = DataHelper::GetInstance().GetTransactions(&fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}

		Sort();
		Filter();

		this->GetEventHandler()->CallAfter(&TransactionsListPanel::UpdateList);
	}).detach();
}

void TransactionsListPanel::Sort() {
	std::sort(transactions.begin(), transactions.end(), [this](const std::shared_ptr<Transaction>& v1, const std::shared_ptr<Transaction>& v2) {
		if (this->sortBy == 0) {
			return v1->fromAccount->name->Cmp(*v2->fromAccount->name) == 0;
		}
		else if (this->sortBy == 1) {
			wxString s = v1->GetTagsString().Lower();
			wxString s1 = v2->GetTagsString().Lower();
			return s.CmpNoCase(s1) == 0;
		}
		else if (this->sortBy == 2) {
			return v1->paidAt->GetMillisecond() < v2->paidAt->GetMillisecond();
		}
		else {
			return v1->fromAmount < v2->fromAmount;
		}
	});

	if (sortDesc) {
		std::reverse(transactions.begin(), transactions.end());
	}
}

void TransactionsListPanel::Filter() {
	filtered.clear();

	if (searchField->GetValue().Length() > 0) {
		wxString search = searchField->GetValue();

		auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

		std::wstring searchW = search.ToStdWstring();
		f.tolower(&searchW[0], &searchW[0] + searchW.size());

		for (auto transaction : transactions)
		{
			wxString fromName = *transaction->fromAccount->name;
			wxString toName = *transaction->toAccount->name;			
			wxString tags = transaction->GetTagsString();
			wxString note = *transaction->note;			

			wxString searchString = fromName + " " + toName + " " + tags + " " + note;

			std::wstring searchStringW = searchString.ToStdWstring();
			f.tolower(&searchStringW[0], &searchStringW[0] + searchStringW.size());

			std::size_t found = searchStringW.find(searchW);

			if (found != std::string::npos) {
				filtered.push_back(transaction);
			}
		}
	}
	else {
		filtered = transactions;
	}
}

void TransactionsListPanel::CreateListColumns() {
	ListColumnsTypes columnsType = ListColumnsTypes::All;

	if (this->type == TreeMenuItemTypes::MenuAccount && this->account) {
		if (account->type == AccountTypes::Receipt) {
			columnsType = ListColumnsTypes::Receipts;
		}
		else if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Virtual) {
			columnsType = ListColumnsTypes::Deposits;
		}
		else {
			columnsType = ListColumnsTypes::Expenses;
		}
	}

	list->ClearColumns();

	auto &columns = Settings::GetInstance().GetTransactionsListColumns(columnsType);

	for (auto &column : columns) {
		switch (static_cast<TransactionsListDataModel::Columns>(column.index))
		{
			case TransactionsListDataModel::Columns::Date:
				list->AppendTextColumn("Date", static_cast<int>(TransactionsListDataModel::Columns::Date), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::FromAccount:
				list->AppendTextColumn("From Account", static_cast<int>(TransactionsListDataModel::Columns::FromAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::ToAccount:
				list->AppendTextColumn("To Account", static_cast<int>(TransactionsListDataModel::Columns::ToAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Tags:
				list->AppendTextColumn("Tags", static_cast<int>(TransactionsListDataModel::Columns::Tags), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Note:
				list->AppendTextColumn("Note", static_cast<int>(TransactionsListDataModel::Columns::Note), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Amount:
				list->AppendTextColumn("Amount", static_cast<int>(TransactionsListDataModel::Columns::Amount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
		}
	}
	
	list->AppendTextColumn("", static_cast<int>(TransactionsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void TransactionsListPanel::UpdateList() {
	model.get()->SetItems(filtered);
	UpdateInfo();
}

void TransactionsListPanel::UpdateInfo() {
	float income = 0;
	float outcome = 0;

	for (auto transaction : filtered)
	{
		if (this->type == TreeMenuItemTypes::MenuAccount) {
			if (account->type == AccountTypes::Deposit) {
				income = income + transaction->toAmount;
				outcome = outcome + transaction->fromAmount;
			}
			else if (account->type == AccountTypes::Receipt) {
				income = income + transaction->toAmount;
			}
			else if (account->type == AccountTypes::Expens) {
				outcome = outcome + transaction->toAmount;
			}
		}
		else if (type == TreeMenuItemTypes::MenuAccounts) {
			if (transaction->toAccount->type == AccountTypes::Expens) {
				outcome = outcome + transaction->toAmount;
			}			
			
			if (transaction->fromAccount->type == AccountTypes::Receipt) {
				income = income + transaction->toAmount;
			}
		}
		else if (this->type == TreeMenuItemTypes::MenuExpenses) {
			outcome = outcome + transaction->toAmount;
		}
		else if (this->type == TreeMenuItemTypes::MenuReceipts) {
			income = income + transaction->toAmount;
		}
		else if (this->type == TreeMenuItemTypes::MenuDeposits) {
			income = income + transaction->toAmount;
			outcome = outcome + transaction->fromAmount;
		}
	}

	transactionLabel->SetLabel(wxString::Format("%d", filtered.size()));
	incomeLabel->SetLabel(wxNumberFormatter::ToString(income, 2));
	outcomeLabel->SetLabel(wxNumberFormatter::ToString(outcome, 2));

	infoPanel->Layout();
}

void TransactionsListPanel::Add() {
	if (OnAdd) {
		OnAdd();
	}
}

void TransactionsListPanel::Edit() {
	if (OnEdit) {
		OnEdit(GetTransaction());
	}
}

void TransactionsListPanel::Copy() {
	if (OnCopy) {
		OnCopy(GetTransaction());
	}
}

void TransactionsListPanel::Delete() {
	auto transaction = GetTransaction();

	if (transaction) {
		transaction->Delete();
		Update();
	}
}

void TransactionsListPanel::Duplicate() {
	auto transaction = GetTransaction();

	if (transaction) {
		Transaction *copy = new Transaction();

		copy->fromAccount = transaction->fromAccount;
		copy->toAccount = transaction->toAccount;
		copy->fromAmount = transaction->fromAmount;
		copy->toAmount = transaction->toAmount;
		copy->note = transaction->note;
		copy->tags = transaction->tags;
		copy->paidAt = transaction->paidAt;

		copy->Save();

		delete copy;

		Update();
	}
}

void TransactionsListPanel::Split() {
	if (OnSplit) {
		OnSplit(GetTransaction());
	}
}

void TransactionsListPanel::Merge() {	
	vector<shared_ptr<Transaction>> _transactions;

	wxDataViewItemArray selections;

	list->GetSelections(selections);

	for (unsigned int i = 0; i < selections.GetCount(); i++) {
		wxDataViewItem item = selections[i];
		
		if (item.IsOk()) {
			int index = (int)item.GetID() - 1;
			_transactions.push_back(filtered[index]);
		}
	}

	if (_transactions.size() > 1) {
		auto firstTransaction = _transactions[0];
		auto tags = firstTransaction->tags;
		std::vector<wxString> newTags;

		for (unsigned int i = 1; i < _transactions.size(); i++) {
			auto transaction = _transactions[i];

			firstTransaction->fromAmount = firstTransaction->fromAmount + transaction->fromAmount;
			firstTransaction->toAmount = firstTransaction->toAmount + transaction->toAmount;
			
			auto _tags = transaction->tags;

			for (auto _tag : _tags)
			{
				for (auto tag : tags)
				{
					if (_tag != tag) {
						newTags.push_back(_tag);
					}
				}
			}

			transaction->Delete();
		}

		firstTransaction->tags = newTags;
		firstTransaction->Save();

		Update();
	}	
}

void TransactionsListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	if (OnEdit) {
		OnEdit(GetTransaction());
	}
}

void TransactionsListPanel::OnListColumnClick(wxListEvent &event) {
	if (sortBy == event.GetColumn()) {
		sortDesc = !sortDesc;
	}

	sortBy = event.GetColumn();
	Update();
}

void TransactionsListPanel::OnColumnDragged(wxListEvent &event) {
	wxListItem item = event.GetItem();
	wxListItem column;

	//transactionsList->GetColumn(event.GetColumn(), column);
}

void TransactionsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *copyItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Copy), wxT("Copy..."));
	wxMenuItem *duplicateItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Duplicate), wxT("Dublicate"));
	wxMenuItem *splitItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Split), wxT("Split..."));
	wxMenuItem *mergeItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Merge), wxT("Merge"));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(TransactionsPanelMenuTypes::Delete), wxT("Delete"));

	addItem->Enable(true);
	editItem->Enable(true);
	copyItem->Enable(true);
	duplicateItem->Enable(true);
	splitItem->Enable(true);
	mergeItem->Enable(true);
	deleteItem->Enable(true);

	if (list->GetSelectedItemsCount() == 0) {
		editItem->Enable(false);
		editItem->SetTextColour(*wxLIGHT_GREY);

		copyItem->Enable(false);
		copyItem->SetTextColour(*wxLIGHT_GREY);

		copyItem->Enable(false);
		copyItem->SetTextColour(*wxLIGHT_GREY);

		duplicateItem->Enable(false);
		duplicateItem->SetTextColour(*wxLIGHT_GREY);

		splitItem->Enable(false);
		splitItem->SetTextColour(*wxLIGHT_GREY);

		mergeItem->Enable(false);
		mergeItem->SetTextColour(*wxLIGHT_GREY);

		deleteItem->Enable(false);
		deleteItem->SetTextColour(*wxLIGHT_GREY);
	}

	if (list->GetSelectedItemsCount() < 2) {
		mergeItem->Enable(false);
		mergeItem->SetTextColour(*wxLIGHT_GREY);
	}

	menu->Append(addItem);
	menu->Append(editItem);
	menu->Append(copyItem);
	menu->Append(duplicateItem);
	menu->AppendSeparator();
	menu->Append(splitItem);
	menu->Append(mergeItem);
	menu->AppendSeparator();
	menu->Append(deleteItem);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionsListPanel::OnMenuSelect, this);

	list->PopupMenu(menu);

	delete menu;
}

void TransactionsListPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<TransactionsPanelMenuTypes>(event.GetId())) {
		case TransactionsPanelMenuTypes::Add:
			Add();
			break;

		case TransactionsPanelMenuTypes::Edit:
			Edit();
			break;

		case TransactionsPanelMenuTypes::Copy:
			Copy();
			break;

		case TransactionsPanelMenuTypes::Duplicate:
			Duplicate();
			break;

		case TransactionsPanelMenuTypes::Split:
			Split();
			break;

		case TransactionsPanelMenuTypes::Merge:
			Merge();
			break;

		case TransactionsPanelMenuTypes::Delete:
			Delete();
			break;

		default:
			break;
	}
}

void TransactionsListPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();

	if (OnPeriodChanged) {
		OnPeriodChanged();
	}
}

void TransactionsListPanel::OnDateChanged(wxDateEvent &event) {
	periodFromDate = GetFromDate();
	periodToDate = GetToDate();

	SaveFilterSettings();

	Update();

	if (OnPeriodChanged) {
		OnPeriodChanged();
	}
}

void TransactionsListPanel::OnSearchChanged(wxCommandEvent &event) {
	Filter();
	UpdateList();
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
		fromDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun).SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);
		toDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun);
		break;

	case 1:
		fromDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);
		toDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Sun);
		break;

	case 2:
		fromDate.Subtract(wxDateSpan::wxDateSpan(0, 1, 0, 0)).SetDay(1);
		toDate.Subtract(wxDateSpan::wxDateSpan(0, 1, 0, 0)).SetToLastMonthDay();
		break;

	case 3:
		fromDate.SetDay(1);
		toDate.SetToLastMonthDay();
		break;

	case 4:
		fromDate.SetMonth(wxDateTime::Month::Jan);
		fromDate.SetDay(1);
		toDate.SetMonth(wxDateTime::Month::Dec);
		toDate.SetDay(31);
		break;

	case 5:
		fromDate = periodFromDate;
		toDate = periodToDate;

		fromDatePicker->Enable();
		toDatePicker->Enable();
		break;

	default:
		break;
	}

	fromDatePicker->SetValue(fromDate);
	toDatePicker->SetValue(toDate);
}

float TransactionsListPanel::GetBalance() {
	return balance;
}

void TransactionsListPanel::RestoreFilterSettings() {
	int id = -1;

	if (account) {
		id = account->id;
	}

	ListFilterSettings settings = Settings::GetInstance().GetListFilterSettings(static_cast<int>(type), id);

	periodFromDate = settings.fromDate;
	periodToDate = settings.toDate;

	periodList->SetSelection(settings.period);
	CalculatePeriod();
}

void TransactionsListPanel::SaveFilterSettings() {
	int id = -1;

	if (account)
	{
		id = account->id;
	}

	Settings::GetInstance().SetListFilterSettings(static_cast<int>(type), id, periodList->GetSelection(), periodFromDate, periodToDate);
}

void TransactionsListPanel::SaveColumnsSettings() {
	ListColumnsTypes columnsType = ListColumnsTypes::All;

	if (this->type == TreeMenuItemTypes::MenuAccount) {
		if (account->type == AccountTypes::Receipt) {
			columnsType = ListColumnsTypes::Receipts;
		}
		else if (account->type == AccountTypes::Deposit || account->type == AccountTypes::Virtual) {
			columnsType = ListColumnsTypes::Deposits;
		}
		else {
			columnsType = ListColumnsTypes::Expenses;
		}
	}

	auto columns = Settings::GetInstance().GetTransactionsListColumns(columnsType);

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn *column = list->GetColumn(i);

		columns[i].index = list->GetColumnIndex(column);
		columns[i].order = list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
	}

	Settings::GetInstance().SetTransactionsListColumns(columnsType, columns);
}