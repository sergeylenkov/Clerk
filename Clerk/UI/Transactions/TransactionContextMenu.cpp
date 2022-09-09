#include "TransactionContextMenu.h"

TransactionContextMenu::TransactionContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<TransactionViewModel> transaction) : _commandsInvoker(commandsInvoker), _transaction(transaction) {
	if (transaction) {
		wxMenuItem* item = this->Append(static_cast<int>(TransactionContextMenuType::Add), wxT("Add..."));
		item->SetBitmap(wxBitmap(wxT("ICON_ADD"), wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Edit), wxT("Edit..."));
		item->SetBitmap(wxBitmap(wxT("ICON_EDIT"), wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Copy), wxT("Copy..."));
		item->SetBitmap(wxBitmap(wxT("ICON_COPY"), wxBITMAP_TYPE_PNG_RESOURCE));

		this->Append(static_cast<int>(TransactionContextMenuType::Duplicate), wxT("Dublicate"));
		this->AppendSeparator();

		item = this->Append(static_cast<int>(TransactionContextMenuType::Split), wxT("Split..."));
		item->SetBitmap(wxBitmap(wxT("ICON_SPLIT"), wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TransactionContextMenuType::Merge), wxT("Merge"));
		item->SetBitmap(wxBitmap(wxT("ICON_MERGE"), wxBITMAP_TYPE_PNG_RESOURCE));

		this->AppendSeparator();

		item = this->Append(static_cast<int>(TransactionContextMenuType::Delete), wxT("Delete"));
		item->SetBitmap(wxBitmap(wxT("ICON_DELETE"), wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		wxMenuItem* item = this->Append(static_cast<int>(TransactionContextMenuType::Add), wxT("Add..."));
		item->SetBitmap(wxBitmap(wxT("ICON_ADD"), wxBITMAP_TYPE_PNG_RESOURCE));
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionContextMenu::OnMenuSelect, this);
}

void TransactionContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TransactionContextMenuType type = static_cast<TransactionContextMenuType>(event.GetId());
	int id = -1;

	if (_transaction) {
		id = _transaction.get()->id;
	}

	switch (type)
	{
		case TransactionContextMenuType::Add:
			_commandsInvoker.OnNewTransaction(id);
			break;
		case TransactionContextMenuType::Edit:
			_commandsInvoker.OnCopyTransaction(id);
			break;
		default:
			break;
	}	
}