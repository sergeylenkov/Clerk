#include "TransactionsListPanel.h"

TransactionsListPanel::TransactionsListPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataListPanel(parent, context, icons)
{
	_model = new TransactionsListDataModel();
	_list->AssociateModel(_model.get());

	int indent = FromDIP(5);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(-1, 40)));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	_periodFilterPanel = new PeriodFilterPanel(filterPanel);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);

	wxBoxSizer *searchSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *searchLabel = new wxStaticText(filterPanel, wxID_ANY, _("Search:"));
	_searchField = new wxTextCtrl(filterPanel, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(300, 20)));
	_searchField->Bind(wxEVT_TEXT, &TransactionsListPanel::OnSearchChanged, this);

	searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);
	searchSizer->Add(_searchField, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	filterSizer->Add(searchSizer, 0, wxALIGN_CENTER_VERTICAL);

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

	_mainSizer->Insert(0, filterPanel, 0, wxEXPAND);
	_mainSizer->Insert(1, _infoPanel, 0, wxEXPAND);	

	Layout();

	_accountType = std::nullopt;
	_balance = 0.0;

	_transactionsService = &_context.GetTransactionsService();

	_subscriptionId = _transactionsService->Subscribe([&]() {
		Update();
	});

	RestoreFilterSettings();
	CreateListColumns();
}

TransactionsListPanel::~TransactionsListPanel() {
	_transactionsService->Unsubscribe(_subscriptionId);

	SaveFilterSettings();
	SaveColumnsSettings();	
}

void TransactionsListPanel::SetAccount(std::shared_ptr<AccountPresentationModel> account) {
	_account = account;

	CreateListColumns();
}

void TransactionsListPanel::SetAccountType(AccountType type) {
	_accountType = type;

	CreateListColumns();
}

void TransactionsListPanel::Update() {
	std::thread([&]()
	{
		_transactions.clear();

		auto transactions = _transactionsService->GetForPeriod(_periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());

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

		for (auto& transaction : _transactions)
		{		
			wxString tags = transaction->tagsString;
			wxString note = transaction->note;			

			wxString rowString = transaction->fromAccount->name + " " + transaction->toAccount->name + " " + tags + " " + note;

			if (String::Search(rowString, search)) {
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

		switch (static_cast<TransactionsListColumns>(column.model))
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
	}
	
	_list->AppendTextColumn("", static_cast<int>(TransactionsListColumns::Last), wxDATAVIEW_CELL_INERT, 10, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE);

	RestoreSorting(columns);
}

void TransactionsListPanel::SaveColumnsSettings() {
	TransactionsListType listType = GetListType();

	auto columns = Settings::GetInstance().GetTransactionsListColumns(listType);
	columns = UpdateColumnsSettings(columns);

	Settings::GetInstance().SetTransactionsListColumns(listType, columns);
}

void TransactionsListPanel::UpdateList() {
	_model->SetItems(_filtered);
	UpdateInfo();
}

void TransactionsListPanel::UpdateInfo() {
	TransactionsListType listType = GetListType();

	float income = 0;
	float outcome = 0;

	for (auto& transaction : _filtered) {
		if (listType == TransactionsListType::Receipts) {
			income = income + transaction->fromAmount;
		} else if (listType == TransactionsListType::Expenses) {
			outcome = outcome + transaction->toAmount;
		}
		else {
			if (transaction->fromAccount->type == AccountType::Receipt || transaction->fromAccount->type == AccountType::Deposit) {
				income = income + transaction->toAmount;
			}

			if (transaction->toAccount->type == AccountType::Expens || transaction->toAccount->type == AccountType::Debt) {
				outcome = outcome + transaction->fromAmount;
			}
		}
	}

	_transactionLabel->SetLabel(wxString::Format("%d", static_cast<int>(_filtered.size())));
	_incomeLabel->SetLabel(Format::Amount(income));
	_outcomeLabel->SetLabel(Format::Amount(outcome));

	_infoPanel->Layout();
}

void TransactionsListPanel::EditSelectedItem() {
	auto transaction = GetTransaction();

	if (transaction) {
		_context.GetCommandsInvoker().EditTransaction(transaction->id);
	} else {
		_context.GetCommandsInvoker().NewTransaction(-1);
	}
}

void TransactionsListPanel::ShowContextMenu() {
	wxMenu *menu = new TransactionContextMenu(_context.GetCommandsInvoker(), _icons, GetTransaction(), GetSelectedIds());	

	_list->PopupMenu(menu);

	delete menu;
}

void TransactionsListPanel::OnSearchChanged(wxCommandEvent &event) {
	Filter();
	UpdateList();
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

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);
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

	Settings::GetInstance().SetListFilterSettings(accountType, accountId, _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
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