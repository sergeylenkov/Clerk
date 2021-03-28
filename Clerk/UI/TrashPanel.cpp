#include "TrashPanel.h"

TrashPanel::TrashPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, _context) {
	transactionsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	transactionsList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &TrashPanel::OnListItemClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(transactionsList, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

void TrashPanel::Update() {
	transactionsList->ClearAll();
	//transactions = _context.GetTransactionsRepository().GetDeleted();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("From Account"));
	column.SetWidth(200);

	transactionsList->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("To Account"));
	column1.SetWidth(200);

	transactionsList->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("From Amount"));
	column2.SetWidth(100);
	column2.SetAlign(wxLIST_FORMAT_RIGHT);

	transactionsList->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("To Amount"));
	column3.SetWidth(100);
	column3.SetAlign(wxLIST_FORMAT_RIGHT);

	transactionsList->InsertColumn(3, column3);

	wxListItem column4;

	column4.SetId(4);
	column4.SetText(_("Date"));
	column4.SetWidth(200);

	transactionsList->InsertColumn(4, column4);

	int i = 0;

	for (auto &transaction : _transactions)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(transaction->id);

		transactionsList->InsertItem(listItem);
		transactionsList->SetItem(i, 0, transaction->fromAccount->name);
		transactionsList->SetItem(i, 1, transaction->toAccount->name);
		transactionsList->SetItem(i, 2, wxString::Format("%.2f", transaction->fromAmount));
		transactionsList->SetItem(i, 3, wxString::Format("%.2f", transaction->toAmount));
		transactionsList->SetItem(i, 4, transaction->date.Format("%B %d, %Y"));

		i++;
	}
}

std::shared_ptr<TransactionViewModel> TrashPanel::GetTransaction() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = transactionsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		//return transactions[itemIndex];
	}

	return nullptr;
}

void TrashPanel::RestoreTransaction() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = transactionsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}
		else {
			//TODO
			//transactions[itemIndex]->Restore();
		}
	}

	Update();
}

void TrashPanel::DeleteTransaction() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = transactionsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {		
			break;
		} else {
			//TODO
			//transactions[itemIndex]->DeleteCompletely();
			auto transaction = _transactions[itemIndex];
			//_context.GetTransactionsRepository().Delete(*transaction);
		}	
	}

	Update();
}

void TrashPanel::ClearAll() {
	for (auto &transaction : _transactions)
	{
		//transaction->DeleteCompletely();
		//_context.GetTransactionsRepository().Delete(*transaction);
	}

	Update();
}

void TrashPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(static_cast<int>(TrashPanelMenuTypes::Restore), wxT("Restore"));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(TrashPanelMenuTypes::Delete), wxT("Delete..."));
	menu->Append(static_cast<int>(TrashPanelMenuTypes::ClearAll), wxT("Clear All..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TrashPanel::OnMenuSelect, this);

	transactionsList->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void TrashPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<TrashPanelMenuTypes>(event.GetId())) {
		case TrashPanelMenuTypes::Restore:
			RestoreTransaction();
			break;

		case TrashPanelMenuTypes::Delete:
			DeleteTransaction();
			break;

		case TrashPanelMenuTypes::ClearAll:
			ClearAll();
			break;

		default:
			break;
	}
}