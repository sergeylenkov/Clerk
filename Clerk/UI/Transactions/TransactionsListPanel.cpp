#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxBORDER_NONE);

	model = new TransactionsListDataModel();
	_list->AssociateModel(model.get());

	//transactionsList->Bind(wxEVT_LIST_COL_CLICK, &TransactionsListPanel::OnListColumnClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TransactionsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TransactionsListPanel::OnRightClick, this);

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
	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

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

	_context.GetTransactionsService().OnUpdate([this]() {
		Update();
	});
}

TransactionsListPanel::~TransactionsListPanel() {
	SaveFilterSettings();
	SaveColumnsSettings();	
}

std::shared_ptr<TransactionPresentationModel> TransactionsListPanel::GetTransaction() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = (int)item.GetID() - 1;
		return _filtered[index];
	}

	return nullptr;
}

std::vector<int> TransactionsListPanel::GetSelectedIds() {
	std::vector<int> result;

	wxDataViewItemArray selections;

	_list->GetSelections(selections);

	for (unsigned int i = 0; i < selections.GetCount(); i++) {
		wxDataViewItem item = selections[i];

		if (item.IsOk()) {
			int index = (int)item.GetID() - 1;
			result.push_back(_filtered[index]->id);
		}
	}

	return result;
}

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
	std::sort(_transactions.begin(), _transactions.end(), [this](const std::shared_ptr<TransactionPresentationModel>& v1, const std::shared_ptr<TransactionPresentationModel>& v2) {
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

	_list->ClearColumns();

	auto &columns = Settings::GetInstance().GetTransactionsListColumns(columnsType);

	for (auto &column : columns) {
		switch (static_cast<TransactionsListDataModel::Columns>(column.index))
		{
			case TransactionsListDataModel::Columns::Date:
				_list->AppendTextColumn("Date", static_cast<int>(TransactionsListDataModel::Columns::Date), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::FromAccount:
				_list->AppendTextColumn("From Account", static_cast<int>(TransactionsListDataModel::Columns::FromAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::ToAccount:
				_list->AppendTextColumn("To Account", static_cast<int>(TransactionsListDataModel::Columns::ToAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Tags: {
				TransactionsTagsRender *render = new TransactionsTagsRender();

				wxDataViewColumn *dataViewColumn = new wxDataViewColumn("Tags", render, static_cast<int>(TransactionsListDataModel::Columns::Tags), column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);
				}
				break;
			case TransactionsListDataModel::Columns::Note:
				_list->AppendTextColumn("Note", static_cast<int>(TransactionsListDataModel::Columns::Note), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListDataModel::Columns::Amount: {
				TransactionsAmountRender *render = new TransactionsAmountRender();

				wxDataViewColumn *dataViewColumn = new wxDataViewColumn("Amount", render, static_cast<int>(TransactionsListDataModel::Columns::Amount), column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);
				}
				break;
		}
	}
	
	_list->AppendTextColumn("", static_cast<int>(TransactionsListDataModel::Columns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
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

/*void TransactionsListPanel::Merge() {
	vector<shared_ptr<Transaction>> _transactions;

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
	}
}*/

void TransactionsListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	auto transaction = GetTransaction();

	if (transaction) {
		_context.GetCommandsInvoker().OnNewTransaction(transaction->id);
	} else {
		_context.GetCommandsInvoker().OnNewTransaction(-1);
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
	wxMenu *menu = new TransactionContextMenu(_context.GetCommandsInvoker(), GetTransaction(), GetSelectedIds());	

	_list->PopupMenu(menu);

	delete menu;
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