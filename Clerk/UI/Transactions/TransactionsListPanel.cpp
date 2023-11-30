#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxBORDER_NONE);

	_model = new TransactionsListDataModel();
	_list->AssociateModel(_model.get());

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TransactionsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TransactionsListPanel::OnRightClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(-1, 40)));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *periodSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *periodText = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("Current Week"));
	values->Add(wxT("Previous Week"));
	values->Add(wxT("Current Month"));
	values->Add(wxT("Previous Month"));	
	values->Add(wxT("Current Year"));
	values->Add(wxT("Previous Year"));
	values->Add(wxT("Custom"));

	_periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), this->FromDIP(wxSize(120, 20)), *values, wxCB_DROPDOWN | wxCB_READONLY);

	delete values;

	int indent = this->FromDIP(5);

	wxStaticText *fromText = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	_fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), this->FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);
	wxStaticText *toText = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	_toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), this->FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);

	periodSizer->Add(periodText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(_periodList, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(fromText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	periodSizer->Add(_fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(toText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	periodSizer->Add(_toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	filterSizer->Add(periodSizer, 1, wxALIGN_CENTER_VERTICAL);

	wxBoxSizer *searchSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("Search:"));
	_searchField = new wxTextCtrl(filterPanel, wxID_ANY, "", wxPoint(0, 0), this->FromDIP(wxSize(300, 20)));
	_searchField->Bind(wxEVT_TEXT, &TransactionsListPanel::OnSearchChanged, this);

	searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);
	searchSizer->Add(_searchField, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	filterSizer->Add(searchSizer, 0, wxALIGN_CENTER_VERTICAL, 0);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	_infoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(-1, 40)));
	wxBoxSizer *infoSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *transactionText = new wxStaticText(_infoPanel, wxID_ANY, wxT("Transactions:"));
	infoSizer->Add(transactionText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	_transactionLabel = new wxStaticText(_infoPanel, wxID_ANY, wxT("0"));
	infoSizer->Add(_transactionLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	wxStaticText *incomeText = new wxStaticText(_infoPanel, wxID_ANY, wxT("Income:"));
	infoSizer->Add(incomeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);

	_incomeLabel = new wxStaticText(_infoPanel, wxID_ANY, wxT("0.00"));
	infoSizer->Add(_incomeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	wxStaticText *outcomeText = new wxStaticText(_infoPanel, wxID_ANY, wxT("Outcome:"));
	infoSizer->Add(outcomeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);

	_outcomeLabel = new wxStaticText(_infoPanel, wxID_ANY, wxT("0.00"));
	infoSizer->Add(_outcomeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	_infoPanel->SetSizer(infoSizer);
	_infoPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(_infoPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	_periodList->Bind(wxEVT_COMBOBOX, &TransactionsListPanel::OnPeriodSelect, this);
	_fromDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);
	_toDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);

	_periodList->Select(0);

	CalculatePeriod();
	CreateListColumns();

	_balance = 0.0;
	_sortBy = 2;
	_sortDesc = false;

	_transactionsService = &_context.GetTransactionsService();

	_subscriptionId = _transactionsService->Subscribe([&]() {
		Update();
	});
}

TransactionsListPanel::~TransactionsListPanel() {
	_transactionsService->Unsubscribe(_subscriptionId);

	SaveFilterSettings();
	//SaveColumnsSettings();	
}

void TransactionsListPanel::SetAccount(std::shared_ptr<AccountPresentationModel> account) {
	_account = account;

	Update();
}

void TransactionsListPanel::SetAccountType(AccountType type) {
	_accountType = type;

	Update();
}

void TransactionsListPanel::Update() {
	std::thread([&]()
	{
		RestoreFilterSettings();

		_transactions.clear();

		auto transactions = _transactionsService->GetForPeriod(_fromDatePicker->GetValue(), _toDatePicker->GetValue());

		if (!_account) {
			_transactions = transactions;
		}
		else if (_account) {
			std::copy_if(transactions.begin(), transactions.end(), std::back_inserter(_transactions), [&](const std::shared_ptr<TransactionPresentationModel>& transaction) {
				return transaction->toAccount->id == _account->id || transaction->fromAccount->id == _account->id;
			});
		}
		else {
			std::copy_if(transactions.begin(), transactions.end(), std::back_inserter(_transactions), [&](const std::shared_ptr<TransactionPresentationModel>& transaction) {
				return transaction->toAccount->type == _accountType || transaction->fromAccount->type == _accountType;
			});
		}

		Sort();
		Filter();

		this->GetEventHandler()->CallAfter(&TransactionsListPanel::UpdateList);
	}).detach();
}

void TransactionsListPanel::Sort() {
	std::sort(_transactions.begin(), _transactions.end(), [&](const std::shared_ptr<TransactionPresentationModel>& v1, const std::shared_ptr<TransactionPresentationModel>& v2) {
		if (_sortBy == 0) {
			return v1->fromAccount->name.Cmp(v2->fromAccount->name) == 0;
		}
		else if (_sortBy == 1) {
			wxString s = v1->tagsString.Lower();
			wxString s1 = v2->tagsString.Lower();
			return s.CmpNoCase(s1) == 0;
		}
		else if (_sortBy == 2) {
			return v1->date.GetMillisecond() < v2->date.GetMillisecond();
		}
		else {
			return v1->fromAmount < v2->fromAmount;
		}
	});

	if (_sortDesc) {
		std::reverse(_transactions.begin(), _transactions.end());
	}
}

void TransactionsListPanel::Filter() {
	_filtered.clear();

	if (_searchField->GetValue().Length() > 0) {
		wxString search = _searchField->GetValue();

		auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

		std::wstring searchW = search.ToStdWstring();
		f.tolower(&searchW[0], &searchW[0] + searchW.size());

		for (auto& transaction : _transactions)
		{		
			wxString tags = transaction->tagsString;
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

	if (_account) {
		switch (_account->type)
		{
			case AccountType::Receipt:
				columnsType = ListColumnsTypes::Receipts;
				break;
			case AccountType::Deposit:
				columnsType = ListColumnsTypes::Deposits;
				break;
			case AccountType::Expens:
				columnsType = ListColumnsTypes::Expenses;
				break;
			case AccountType::Debt:
				columnsType = ListColumnsTypes::Expenses;
				break;
			case AccountType::Virtual:
				columnsType = ListColumnsTypes::Deposits;
				break;
			default:
				break;
		}
	} else {
		switch (_accountType)
		{
			case AccountType::Receipt:
				columnsType = ListColumnsTypes::Receipts;
				break;
			case AccountType::Deposit:
				columnsType = ListColumnsTypes::Deposits;
				break;
			case AccountType::Expens:
				columnsType = ListColumnsTypes::Expenses;
				break;
			case AccountType::Debt:
				columnsType = ListColumnsTypes::Expenses;
				break;
			case AccountType::Virtual:
				columnsType = ListColumnsTypes::Deposits;
				break;
			default:
				break;
		}
	}

	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetTransactionsListColumns(columnsType);

	for (auto& column : columns) {
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
	_model.get()->SetItems(_filtered);
	UpdateInfo();
}

void TransactionsListPanel::UpdateInfo() {
	float income = 0;
	float outcome = 0;

	_transactionLabel->SetLabel(wxString::Format("%d", static_cast<int>(_filtered.size())));
	_incomeLabel->SetLabel(wxNumberFormatter::ToString(income, 2));
	_outcomeLabel->SetLabel(wxNumberFormatter::ToString(outcome, 2));

	_infoPanel->Layout();
}

void TransactionsListPanel::OnListItemDoubleClick(wxDataViewEvent &event) {
	auto transaction = GetTransaction();

	if (transaction) {
		_context.GetCommandsInvoker().OnEditTransaction(transaction->id);
	} else {
		_context.GetCommandsInvoker().OnNewTransaction(-1);
	}
}

void TransactionsListPanel::OnListColumnClick(wxListEvent &event) {
	if (_sortBy == event.GetColumn()) {
		_sortDesc = !_sortDesc;
	}

	_sortBy = event.GetColumn();
	//Update();
}

void TransactionsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new TransactionContextMenu(_context.GetCommandsInvoker(), GetTransaction(), GetSelectedIds());	

	_list->PopupMenu(menu);

	delete menu;
}

void TransactionsListPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	//Update();
}

void TransactionsListPanel::OnDateChanged(wxDateEvent &event) {
	_periodFromDate = _fromDatePicker->GetValue();
	_periodToDate = _toDatePicker->GetValue();

	SaveFilterSettings();

	//Update();
}

void TransactionsListPanel::OnSearchChanged(wxCommandEvent &event) {
	Filter();
	UpdateList();
}

void TransactionsListPanel::CalculatePeriod() {
	int index = _periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	_fromDatePicker->Disable();
	_toDatePicker->Disable();

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
			fromDate = _periodFromDate;
			toDate = _periodToDate;

			_fromDatePicker->Enable();
			_toDatePicker->Enable();
			break;

		default:
			break;
	}

	_fromDatePicker->SetValue(fromDate);
	_toDatePicker->SetValue(toDate);
}

void TransactionsListPanel::RestoreFilterSettings() {
	int id = -1;

	if (_account) {
		id = _account->id;
	}

	ListFilterSettings settings = Settings::GetInstance().GetListFilterSettings(static_cast<int>(_accountType), id);

	_periodFromDate = settings.fromDate;
	_periodToDate = settings.toDate;

	_periodList->SetSelection(settings.period);

	CalculatePeriod();
}

void TransactionsListPanel::SaveFilterSettings() {
	int id = -1;

	if (_account)
	{
		id = _account->id;
	}

	Settings::GetInstance().SetListFilterSettings(static_cast<int>(_accountType), id, _periodList->GetSelection(), _periodFromDate, _periodToDate);
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

std::shared_ptr<TransactionPresentationModel> TransactionsListPanel::GetTransaction() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = reinterpret_cast<int>(item.GetID()) - 1;
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