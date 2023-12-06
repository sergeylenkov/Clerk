#include "TransactionContextMenu.h"

TransactionContextMenu::TransactionContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<TransactionPresentationModel> transaction, std::vector<int> selectedIds):
	_commandsInvoker(commandsInvoker), _transaction(transaction), _selectedIds(selectedIds) {
	if (transaction) {
		wxMenuItem* item = this->Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Edit), _("Edit..."));
		item->SetBitmap(wxBitmap("ICON_EDIT", wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Copy), _("Copy..."));
		item->SetBitmap(wxBitmap("ICON_COPY", wxBITMAP_TYPE_PNG_RESOURCE));

		this->Append(static_cast<int>(TransactionContextMenuType::Duplicate), _("Dublicate"));
		this->AppendSeparator();

		item = this->Append(static_cast<int>(TransactionContextMenuType::Split), _("Split..."));
		item->SetBitmap(wxBitmap("ICON_SPLIT", wxBITMAP_TYPE_PNG_RESOURCE));

		this->AppendSeparator();

		item = this->Append(static_cast<int>(TransactionContextMenuType::Delete), _("Delete"));
		item->SetBitmap(wxBitmap("ICON_DELETE", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else if (_selectedIds.size() > 0) {
		wxMenuItem* item = this->Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Merge), _("Merge"));
		item->SetBitmap(wxBitmap("ICON_MERGE", wxBITMAP_TYPE_PNG_RESOURCE));
	} else {
		wxMenuItem* item = this->Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionContextMenu::OnMenuSelect, this);
}

void TransactionContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TransactionContextMenuType type = static_cast<TransactionContextMenuType>(event.GetId());
	int id = -1;

	if (_transaction) {
		id = _transaction->id;
	}

	switch (type)
	{
		case TransactionContextMenuType::Add:
			_commandsInvoker.OnNewTransaction(id);
			break;
		case TransactionContextMenuType::Edit:
			_commandsInvoker.OnEditTransaction(id);
			break;
		case TransactionContextMenuType::Copy:
			_commandsInvoker.OnCopyTransaction(id);
			break;
		case TransactionContextMenuType::Split:
			_commandsInvoker.OnSplitTransaction(id);
			break;
		case TransactionContextMenuType::Delete:
			_commandsInvoker.OnDeleteTransaction(id);
			break;
		default:
			break;
	}	
}