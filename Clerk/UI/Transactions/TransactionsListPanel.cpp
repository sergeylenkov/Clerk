#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker) : DataPanel(parent, context, commandsInvoker) {
	list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxBORDER_NONE);

	model = new TransactionsListDataModel();
	list->AssociateModel(model.get());

	//transactionsList->Bind(wxEVT_LIST_COL_CLICK, &TransactionsListPanel::OnListColumnClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TransactionsListPanel::OnListItemDoubleClick, this);
	list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TransactionsListPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *periodSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("Current Week"));
	values->Add(wxT("Previous Week"));
	values->Add(wxT("Current Month"));
	values->Add(wxT("Previous Month"));	
	values->Add(wxT("Current Year"));
	values->Add(wxT("Previous Year"));
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

	periodList->Select(0);

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

/*void TransactionsListPanel::SetAccount(std::shared_ptr<AccountModel> account) {
	this->account = account;
}

std::shared_ptr<AccountModel> TransactionsListPanel::GetAccount() {
	return this->account;
}

void TransactionsListPanel::SetType(TreeMenuItemTypes type) {
	this->type = type;
}

std::shared_ptr<TransactionViewModel> TransactionsListPanel::GetTransaction() {
	wxDataViewItem item = list->GetSelection();

	if (item.IsOk()) {
		int index = (int)item.GetID() - 1;
		return _filtered[index];
	}

	return nullptr;
}*/

void TransactionsListPanel::Update() {
	std::thread([=]()
	{
		RestoreFilterSettings();
		//TODO
		/*if (this->type == TreeMenuItemTypes::Account) {
			transactions = DataHelper::GetInstance().GetTransactions(*account, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::Expenses) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountType::Expens, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::Receipts) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountType::Receipt, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::Deposits) {
			transactions = DataHelper::GetInstance().GetTransactionsByType(AccountType::Deposit, &fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}
		else if (this->type == TreeMenuItemTypes::Accounts) {
			transactions = DataHelper::GetInstance().GetTransactions(&fromDatePicker->GetValue(), &toDatePicker->GetValue());
		}*/

		_transactions = _context.GetTransactionsService().GetForPeriod(fromDatePicker->GetValue(), toDatePicker->GetValue());

		Sort();
		Filter();

		this->GetEventHandler()->CallAfter(&TransactionsListPanel::UpdateList);
	}).detach();
}

void TransactionsListPanel::Sort() {
	std::sort(_transactions.begin(), _transactions.end(), [this](const std::shared_ptr<TransactionViewModel>& v1, const std::shared_ptr<TransactionViewModel>& v2) {
		if (this->sortBy == 0) {
			return v1->fromAccount->name.Cmp(v2->fromAccount->name) == 0;
		}
		else if (this->sortBy == 1) {
			//TODO
			wxString s = wxString("");//v1->GetTagsString().Lower();
			wxString s1 = wxString("");//v2->GetTagsString().Lower();
			return s.CmpNoCase(s1) == 0;
		}
		else if (this->sortBy == 2) {
			return v1->date.GetMillisecond() < v2->date.GetMillisecond();
		}
		else {
			return v1->fromAmount < v2->fromAmount;
		}
	});

	if (sortDesc) {
		std::reverse(_transactions.begin(), _transactions.end());
	}
}

void TransactionsListPanel::Filter() {
	_filtered.clear();

	if (searchField->GetValue().Length() > 0) {
		wxString search = searchField->GetValue();

		auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

		std::wstring searchW = search.ToStdWstring();
		f.tolower(&searchW[0], &searchW[0] + searchW.size());

		//TODO
		for (auto &transaction : _transactions)
		{		
			wxString tags = wxString(""); //transaction->GetTagsString();
			wxString note = transaction->note;			

			wxString searchString = transaction->fromAccount->name + " " + transaction->toAccount->name + " " + tags + " " + note;

			std::wstring searchStringW = searchString.ToStdWstring();
			f.tolower(&searchStringW[0], &searchStringW[0] + searchStringW.size());

			std::size_t found = searchStringW.find(searchW);

			if (found != std::string::npos) {
				_filtered.push_back(transaction);
			}
		}
	}
	else {
		_filtered = _transactions;
	}
}

void TransactionsListPanel::CreateListColumns() {
	ListColumnsTypes columnsType = ListColumnsTypes::All;

	if (this->type == TreeMenuItemType::Account && this->account) {
		if (account->type == AccountType::Receipt) {
			columnsType = ListColumnsTypes::Receipts;
		}
		else if (account->type == AccountType::Deposit || account->type == AccountType::Virtual) {
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
			case TransactionsListDataModel::Columns::Tags: {
				TransactionsTagsRender *render = new TransactionsTagsRender();

				wxDataViewColumn *dataViewColumn = new wxDataViewColumn("Tags", render, static_cast<int>(TransactionsListDataModel::Columns::Tags), column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				list->AppendColumn(dataViewColumn);
				}
				break;
			case TransactionsListDataModel::Columns::Note:
				list->AppendTextColumn("Note", static_cast<int>(TransactionsListDataModel::Columns::Note), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Amount: {
				TransactionsAmountRender *render = new TransactionsAmountRender();

				wxDataViewColumn *dataViewColumn = new wxDataViewColumn("Amount", render, static_cast<int>(TransactionsListDataModel::Columns::Amount), column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				list->AppendColumn(dataViewColumn);
				}
				break;
		}
	}
	
	list->AppendTextColumn("", static_cast<int>(TransactionsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void TransactionsListPanel::UpdateList() {
	model.get()->SetItems(_filtered);
	UpdateInfo();
}

void TransactionsListPanel::UpdateInfo() {
	float income = 0;
	float outcome = 0;

	/*for (auto &transaction : _filtered)
	{
		if (this->type == TreeMenuItemTypes::Account) {
			if (account->type == Account::Type::Deposit) {
				income = income + transaction->toAmount;
				outcome = outcome + transaction->fromAmount;
			}
			else if (account->type == Account::Type::Receipt) {
				income = income + transaction->toAmount;
			}
			else if (account->type == Account::Type::Expens) {
				outcome = outcome + transaction->toAmount;
			}
		}
		else if (type == TreeMenuItemTypes::Accounts) {
			if (transaction->toAccount->type == Account::Type::Expens) {
				outcome = outcome + transaction->toAmount;
			}			
			
			if (transaction->fromAccount->type == Account::Type::Receipt) {
				income = income + transaction->toAmount;
			}
		}
		else if (this->type == TreeMenuItemTypes::Expenses) {
			outcome = outcome + transaction->toAmount;
		}
		else if (this->type == TreeMenuItemTypes::Receipts) {
			income = income + transaction->toAmount;
		}
		else if (this->type == TreeMenuItemTypes::Deposits) {
			income = income + transaction->toAmount;
			outcome = outcome + transaction->fromAmount;
		}
	}*/

	transactionLabel->SetLabel(wxString::Format("%d", _filtered.size()));
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
		//OnEdit(GetTransaction());
	}
}

void TransactionsListPanel::Copy() {
	if (OnCopy) {
		//OnCopy(GetTransaction());
	}
}

void TransactionsListPanel::Delete() {
	/*auto transaction = GetTransaction();

	if (transaction) {
		///TODO mark as deleted
		//transaction->Delete();
		Update();
	}*/
}

void TransactionsListPanel::Duplicate() {
	/*auto transaction = GetTransaction();

	if (transaction) {
		Transaction *copy = new Transaction();

		copy->fromAccount = transaction->fromAccount;
		copy->toAccount = transaction->toAccount;
		copy->fromAmount = transaction->fromAmount;
		copy->toAmount = transaction->toAmount;
		copy->note = transaction->note;
		copy->tags = transaction->tags;
		copy->paidAt = transaction->paidAt;

		_context.GetTransactionsRepository().Save(*copy);

		delete copy;

		Update();
	}*/
}

void TransactionsListPanel::Split() {
	if (OnSplit) {
		//OnSplit(GetTransaction());
	}
}

void TransactionsListPanel::Merge() {	
	/*vector<shared_ptr<Transaction>> _transactions;

	wxDataViewItemArray selections;

	list->GetSelections(selections);

	for (unsigned int i = 0; i < selections.GetCount(); i++) {
		wxDataViewItem item = selections[i];
		
		if (item.IsOk()) {
			int index = (int)item.GetID() - 1;
			_transactions.push_back(_filtered[index]);
		}
	}

	if (_transactions.size() > 1) {
		auto firstTransaction = _transactions[0];
		auto tags = firstTransaction.tags;
		std::vector<wxString> newTags;

		for (unsigned int i = 1; i < _transactions.size(); i++) {
			auto transaction = _transactions[i];

			firstTransaction->fromAmount = firstTransaction->fromAmount + transaction->fromAmount;
			firstTransaction->toAmount = firstTransaction->toAmount + transaction->toAmount;
			
			auto _tags = transaction.tags;

			for (auto _tag : _tags)
			{
				for (auto tag : tags)
				{
					if (_tag != tag) {
						newTags.push_back(_tag);
					}
				}
			}

			//transaction->Delete();
		}
		//TODO
		firstTransaction->tags = newTags;
		//firstTransaction->Save();

		Update();
	}	*/
}

void TransactionsListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	if (OnEdit) {
		//OnEdit(GetTransaction());
	}
}

void TransactionsListPanel::OnListColumnClick(wxListEvent &event) {
	if (sortBy == event.GetColumn()) {
		sortDesc = !sortDesc;
	}

	sortBy = event.GetColumn();
	Update();
}

void TransactionsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new wxMenu;

	wxMenuItem *addItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Add), wxT("Add..."));
	wxMenuItem *editItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Edit), wxT("Edit..."));
	wxMenuItem *copyItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Copy), wxT("Copy..."));
	wxMenuItem *duplicateItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Duplicate), wxT("Dublicate"));
	wxMenuItem *splitItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Split), wxT("Split..."));
	wxMenuItem *mergeItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Merge), wxT("Merge"));
	wxMenuItem *deleteItem = new wxMenuItem(menu, static_cast<int>(ContextMenuTypes::Delete), wxT("Delete"));

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
	switch (static_cast<ContextMenuTypes>(event.GetId())) {
		case ContextMenuTypes::Add:
			_commandsInvoker.OnNewTransaction(-1);
			break;

		case ContextMenuTypes::Edit:
			Edit();
			break;

		case ContextMenuTypes::Copy:
			Copy();
			break;

		case ContextMenuTypes::Duplicate:
			Duplicate();
			break;

		case ContextMenuTypes::Split:
			Split();
			break;

		case ContextMenuTypes::Merge:
			Merge();
			break;

		case ContextMenuTypes::Delete:
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
	periodFromDate = fromDatePicker->GetValue();
	periodToDate = toDatePicker->GetValue();

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

void TransactionsListPanel::CalculatePeriod() {
	int index = periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDatePicker->Disable();
	toDatePicker->Disable();

	switch (index)
	{
		case 0:
			Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);
			break;

		case 1:
			Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);
			break;

		case 2:
			Periods::Calculate(Periods::Type::CurrentMonth, fromDate, toDate);
			break;

		case 3:
			Periods::Calculate(Periods::Type::PreviousMonth, fromDate, toDate);
			break;

		case 4:
			Periods::Calculate(Periods::Type::CurrentYear, fromDate, toDate);
			break;

		case 5:
			Periods::Calculate(Periods::Type::PreviousYear, fromDate, toDate);
			break;

		case 6:
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
	/*ListColumnsTypes columnsType = ListColumnsTypes::All;

	if (this->type == TreeMenuItemTypes::Account) {
		if (account->type == Account::Type::Receipt) {
			columnsType = ListColumnsTypes::Receipts;
		}
		else if (account->type == Account::Type::Deposit || account->type == Account::Type::Virtual) {
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

	Settings::GetInstance().SetTransactionsListColumns(columnsType, columns);*/
}