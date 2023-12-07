#include "AccountContextMenu.h"

const int transactionsOffset = 1000;

AccountContextMenu::AccountContextMenu(CommandsInvoker& commandsInvoker, AccountPresentationModel& account, std::vector<std::shared_ptr<TransactionPresentationModel>> transactions, Icons& icons): TreeContextMenu(commandsInvoker),
	_account(account), _icons(icons) {

	wxMenuItem* item = Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));
	item->SetBitmap(wxBitmap("ICON_NEW_TAB", wxBITMAP_TYPE_PNG_RESOURCE));

	AppendSeparator();

	if (!account.isActive) {	
		item = Append(static_cast<int>(TreeContextMenuType::RestoreAccount), _("Restore from Archive"));
		item->SetBitmap(wxBitmap("ICON_UNDO", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		item = Append(static_cast<int>(TreeContextMenuType::EditAccount), _("Edit Account..."));
		item->SetBitmap(wxBitmap("ICON_EDIT", wxBITMAP_TYPE_PNG_RESOURCE));

		item = Append(static_cast<int>(TreeContextMenuType::DeleteAccount), _("Move to Archive"));
		item->SetBitmap(wxBitmap("ICON_DELETE", wxBITMAP_TYPE_PNG_RESOURCE));

		AppendSeparator();

		if (transactions.size() > 0) {
			wxMenu* recentsMenu = new wxMenu();
			AppendSubMenu(recentsMenu, _("New Transaction"));

			item = recentsMenu->Append(static_cast<int>(TreeContextMenuType::NewTransaction), _("New Transaction..."));
			item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

			recentsMenu->AppendSeparator();

			for (auto& transaction : transactions)
			{
				wxMenuItem* transactionItem = recentsMenu->Append(transactionsOffset + transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));

				int iconId = account.icon;

				if (account.id == transaction->fromAccount->id) {
					iconId = transaction->toAccount->icon;
				}
				else {
					iconId = transaction->fromAccount->icon;
				}

				transactionItem->SetBitmap(*_icons.GetAccountIcon(iconId));
			}
		}
		else {
			item = Append(static_cast<int>(TreeContextMenuType::NewTransaction), _("New Transaction..."));
			item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));
		}
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountContextMenu::OnMenuSelect, this);
}

void AccountContextMenu::OnMenuSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id >= transactionsOffset) {
		_commandsInvoker.OnCopyTransaction(id - transactionsOffset);
	} else if (id == static_cast<int>(TreeContextMenuType::NewTransaction)) {
		_commandsInvoker.OnNewTransaction(_account.id);
	}
	else if (id == static_cast<int>(TreeContextMenuType::EditAccount)) {
		_commandsInvoker.OnEditAccount(_account.id);
	} else if (id == static_cast<int>(TreeContextMenuType::NewTab)) {
		_commandsInvoker.OnNewAccountTab(_account.id);
	}
}