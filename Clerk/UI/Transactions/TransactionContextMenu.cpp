#include "TransactionContextMenu.h"

TransactionContextMenu::TransactionContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<TransactionPresentationModel> transaction, std::vector<int> selectedIds):
	_commandsInvoker(commandsInvoker),
	_icons(icons),
	_transaction(transaction),
	_selectedIds(selectedIds)
{
	if (transaction) {
		wxMenuItem* item = Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));

		item = Append(static_cast<int>(TransactionContextMenuType::Edit), _("Edit..."));
		item->SetBitmap(wxBitmap("ICON_EDIT", wxBITMAP_TYPE_PNG_RESOURCE));

		item = Append(static_cast<int>(TransactionContextMenuType::Copy), _("Copy..."));
		item->SetBitmap(wxBitmap("ICON_COPY", wxBITMAP_TYPE_PNG_RESOURCE));

		Append(static_cast<int>(TransactionContextMenuType::Duplicate), _("Dublicate"));
		AppendSeparator();

		item = Append(static_cast<int>(TransactionContextMenuType::Split), _("Split..."));
		item->SetBitmap(wxBitmap("ICON_SPLIT", wxBITMAP_TYPE_PNG_RESOURCE));

		AppendSeparator();

		item = Append(static_cast<int>(TransactionContextMenuType::Delete), _("Delete"));
		item->SetBitmap(wxBitmap("ICON_DELETE", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else if (_selectedIds.size() > 0) {
		wxMenuItem* item = Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Tab));

		item = Append(static_cast<int>(TransactionContextMenuType::Merge), _("Merge"));
		item->SetBitmap(wxBitmap("ICON_MERGE", wxBITMAP_TYPE_PNG_RESOURCE));
	} else {
		wxMenuItem* item = Append(static_cast<int>(TransactionContextMenuType::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Tab));
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionContextMenu::OnMenuSelect, this);
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
			_commandsInvoker.NewTransaction(id);
			break;
		case TransactionContextMenuType::Edit:
			_commandsInvoker.EditTransaction(id);
			break;
		case TransactionContextMenuType::Copy:
			_commandsInvoker.CopyTransaction(id);
			break;
		case TransactionContextMenuType::Split:
			_commandsInvoker.SplitTransaction(id);
			break;
		case TransactionContextMenuType::Delete:
			_commandsInvoker.DeleteTransaction(id);
			break;
		default:
			break;
	}	
}