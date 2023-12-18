#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons) {
	_list = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxBORDER_NONE);

	_model = new TransactionsListDataModel();
	_list->AssociateModel(_model.get());

	_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TransactionsListPanel::OnListItemDoubleClick, this);
	_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TransactionsListPanel::OnRightClick, this);
	_list->Bind(wxEVT_DATAVIEW_COLUMN_SORTED, &TransactionsListPanel::OnListColumnClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(-1, 40)));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *periodSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *periodText = new wxStaticText(filterPanel, wxID_ANY, _("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(_("Current Week"));
	values->Add(_("Previous Week"));
	values->Add(_("Current Month"));
	values->Add(_("Previous Month"));	
	values->Add(_("Current Year"));
	values->Add(_("Previous Year"));
	values->Add(_("Custom"));

	_periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), FromDIP(wxSize(120, 20)), *values, wxCB_DROPDOWN | wxCB_READONLY);

	delete values;

	int indent = FromDIP(5);

	wxStaticText *fromText = new wxStaticText(filterPanel, wxID_ANY, _("From:"));
	_fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);
	wxStaticText *toText = new wxStaticText(filterPanel, wxID_ANY, _("To:"));
	_toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);

	periodSizer->Add(periodText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(_periodList, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(fromText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	periodSizer->Add(_fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	periodSizer->Add(toText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	periodSizer->Add(_toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	filterSizer->Add(periodSizer, 1, wxALIGN_CENTER_VERTICAL);

	wxBoxSizer *searchSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(filterPanel, wxID_ANY, _("Search:"));
	_searchField = new wxTextCtrl(filterPanel, wxID_ANY, "", wxPoint(0, 0), FromDIP(wxSize(300, 20)));
	_searchField->Bind(wxEVT_TEXT, &TransactionsListPanel::OnSearchChanged, this);

	searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);
	searchSizer->Add(_searchField, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	filterSizer->Add(searchSizer, 0, wxALIGN_CENTER_VERTICAL, 0);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	_infoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(-1, 40)));
	wxBoxSizer *infoSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *transactionText = new wxStaticText(_infoPanel, wxID_ANY, _("Transactions:"));
	infoSizer->Add(transactionText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	_transactionLabel = new wxStaticText(_infoPanel, wxID_ANY, "0");
	infoSizer->Add(_transactionLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	wxStaticText *incomeText = new wxStaticText(_infoPanel, wxID_ANY, _("Income:"));
	infoSizer->Add(incomeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);

	_incomeLabel = new wxStaticText(_infoPanel, wxID_ANY, "0.00");
	infoSizer->Add(_incomeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	wxStaticText *outcomeText = new wxStaticText(_infoPanel, wxID_ANY, _("Outcome:"));
	infoSizer->Add(outcomeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);

	_outcomeLabel = new wxStaticText(_infoPanel, wxID_ANY, "0.00");
	infoSizer->Add(_outcomeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	_infoPanel->SetSizer(infoSizer);
	_infoPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(_infoPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(_list, 1, wxALL | wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();

	_periodList->Bind(wxEVT_COMBOBOX, &TransactionsListPanel::OnPeriodSelect, this);
	_fromDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);
	_toDatePicker->Bind(wxEVT_DATE_CHANGED, &TransactionsListPanel::OnDateChanged, this);

	_periodList->Select(0);	

	_accountType = std::nullopt;
	_balance = 0.0;
	_sortBy = TransactionsListColumns::Date;
	_sortDesc = false;

	_transactionsService = &_context.GetTransactionsService();

	_subscriptionId = _transactionsService->Subscribe([&]() {
		Update();
	});
}

TransactionsListPanel::~TransactionsListPanel() {
	_transactionsService->Unsubscribe(_subscriptionId);

	SaveFilterSettings();
	SaveColumnsSettings();	
}

void TransactionsListPanel::SetAccount(std::shared_ptr<AccountPresentationModel> account) {
	_account = account;
}

void TransactionsListPanel::SetAccountType(AccountType type) {
	_accountType = type;
}

void TransactionsListPanel::Update() {
	RestoreFilterSettings();
	CalculatePeriod();
	CreateListColumns();

	std::thread([&]()
	{
		_transactions.clear();

		auto transactions = _transactionsService->GetForPeriod(_fromDatePicker->GetValue(), _toDatePicker->GetValue());

		if (_account) {
			std::copy_if(transactions.begin(), transactions.end(), std::back_inserter(_transactions), [&](const std::shared_ptr<TransactionPresentationModel>& transaction) {
				return transaction->toAccount->id == _account->id || transaction->fromAccount->id == _account->id;
				});
		} else if (_accountType.has_value()) {		
			std::copy_if(transactions.begin(), transactions.end(), std::back_inserter(_transactions), [&](const std::shared_ptr<TransactionPresentationModel>& transaction) {
				return transaction->toAccount->type == _accountType || transaction->fromAccount->type == _accountType;
			});
		}
		else {
			_transactions = transactions;
		}

		Filter();

		GetEventHandler()->CallAfter(&TransactionsListPanel::UpdateList);
	}).detach();
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
	TransactionsListType listType = GetListType();

	_list->ClearColumns();

	auto& columns = Settings::GetInstance().GetTransactionsListColumns(listType);

	for (auto& column : columns) {
		wxDataViewColumn* dataViewColumn = nullptr;

		switch (static_cast<TransactionsListColumns>(column.index))
		{
			case TransactionsListColumns::Date:
				dataViewColumn = _list->AppendTextColumn(_("Date"), static_cast<int>(TransactionsListColumns::Date), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_CENTER, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListColumns::FromAccount:
				dataViewColumn = _list->AppendTextColumn(_("From Account"), static_cast<int>(TransactionsListColumns::FromAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListColumns::ToAccount:
				dataViewColumn = _list->AppendTextColumn(_("To Account"), static_cast<int>(TransactionsListColumns::ToAccount), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListColumns::Tags: {
				TransactionsTagsRender *render = new TransactionsTagsRender();

				dataViewColumn = new wxDataViewColumn(_("Tags"), render, static_cast<int>(TransactionsListColumns::Tags), column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);
				}
				break;
			case TransactionsListColumns::Note:
				dataViewColumn = _list->AppendTextColumn(_("Note"), static_cast<int>(TransactionsListColumns::Note), wxDATAVIEW_CELL_INERT, column.width, wxALIGN_NOT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				break;
			case TransactionsListColumns::Amount: {
				TransactionsAmountRender *render = new TransactionsAmountRender();

				dataViewColumn = new wxDataViewColumn(_("Amount"), render, static_cast<int>(TransactionsListColumns::Amount), column.width, wxALIGN_RIGHT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
				_list->AppendColumn(dataViewColumn);				
				}
				break;
		}

		if (column.sorted) {
			_sortBy = static_cast<TransactionsListColumns>(column.index);
			_sortDesc = column.sortedDesc;

			if (dataViewColumn != nullptr) {
				dataViewColumn->SetSortOrder(!_sortDesc);
			}
		}
	}
	
	_list->AppendTextColumn("", static_cast<int>(TransactionsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);
}

void TransactionsListPanel::SaveColumnsSettings() {
	TransactionsListType listType = GetListType();

	auto columns = Settings::GetInstance().GetTransactionsListColumns(listType);

	for (unsigned int i = 0; i < columns.size(); i++) {
		wxDataViewColumn* column = _list->GetColumn(i);

		columns[i].index = _list->GetColumnIndex(column);
		columns[i].order = _list->GetColumnPosition(column);
		columns[i].width = column->GetWidth();
		columns[i].sorted = columns[i].index == static_cast<int>(_sortBy);
		columns[i].sortedDesc = _sortDesc;
	}

	Settings::GetInstance().SetTransactionsListColumns(listType, columns);
}

void TransactionsListPanel::UpdateList() {
	_model->SetItems(_filtered);
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
		_context.GetCommandsInvoker().EditTransaction(transaction->id);
	} else {
		_context.GetCommandsInvoker().NewTransaction(-1);
	}
}

void TransactionsListPanel::OnListColumnClick(wxDataViewEvent &event) {
	int index = event.GetColumn();
	wxDataViewColumn* column = event.GetDataViewColumn();

	_sortDesc = !column->IsSortOrderAscending();
	_sortBy = static_cast<TransactionsListColumns>(index);
}

void TransactionsListPanel::OnRightClick(wxDataViewEvent &event) {
	wxMenu *menu = new TransactionContextMenu(_context.GetCommandsInvoker(), _icons, GetTransaction(), GetSelectedIds());	

	_list->PopupMenu(menu);

	delete menu;
}

void TransactionsListPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();
}

void TransactionsListPanel::OnDateChanged(wxDateEvent &event) {
	_periodFromDate = _fromDatePicker->GetValue();
	_periodToDate = _toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
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
	int accountId = -1;
	int accountType = -1;

	if (_account) {
		accountId = _account->id;
	}

	if (_accountType.has_value()) {
		accountType = static_cast<int>(_accountType.value());
	}

	ListFilterSettings settings = Settings::GetInstance().GetListFilterSettings(accountType, accountId);

	_periodFromDate = settings.fromDate;
	_periodToDate = settings.toDate;

	_periodList->SetSelection(settings.period);

	CalculatePeriod();
}

void TransactionsListPanel::SaveFilterSettings() {
	int accountId = -1;
	int accountType = -1;

	if (_account)
	{
		accountId = _account->id;
	}

	if (_accountType.has_value()) {
		accountType = static_cast<int>(_accountType.value());
	}

	Settings::GetInstance().SetListFilterSettings(accountType, accountId, _periodList->GetSelection(), _periodFromDate, _periodToDate);
}

std::shared_ptr<TransactionPresentationModel> TransactionsListPanel::GetTransaction() {
	wxDataViewItem item = _list->GetSelection();

	if (item.IsOk()) {
		int index = wxPtrToUInt(item.GetID()) - 1;
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
			int index = wxPtrToUInt(item.GetID()) - 1;
			result.push_back(_filtered[index]->id);
		}
	}

	return result;
}

TransactionsListType TransactionsListPanel::GetListType() {
	TransactionsListType type = TransactionsListType::All;

	if (_account) {
		switch (_account->type)
		{
		case AccountType::Receipt:
			type = TransactionsListType::Receipts;
			break;
		case AccountType::Deposit:
			type = TransactionsListType::Deposits;
			break;
		case AccountType::Expens:
			type = TransactionsListType::Expenses;
			break;
		case AccountType::Debt:
			type = TransactionsListType::Expenses;
			break;
		case AccountType::Virtual:
			type = TransactionsListType::Deposits;
			break;
		default:
			break;
		}
	}
	else if (_accountType.has_value()) {
		switch (_accountType.value())
		{
		case AccountType::Receipt:
			type = TransactionsListType::Receipts;
			break;
		case AccountType::Deposit:
			type = TransactionsListType::Deposits;
			break;
		case AccountType::Expens:
			type = TransactionsListType::Expenses;
			break;
		case AccountType::Debt:
			type = TransactionsListType::Expenses;
			break;
		case AccountType::Virtual:
			type = TransactionsListType::Deposits;
			break;
		default:
			break;
		}
	}

	return type;
}