#include "AccountContextMenu.h"

const int transactionsOffset = 1000;

AccountContextMenu::AccountContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, AccountPresentationModel& account, std::vector<std::shared_ptr<TransactionPresentationModel>> transactions):
	TreeContextMenu(commandsInvoker, icons),
	_account(account)
{
	wxMenuItem* item = Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));	
	item->SetBitmap(_icons.GetIconByType(IconType::Tab));

	AppendSeparator();

	if (!account.isActive) {	
		item = Append(static_cast<int>(TreeContextMenuType::RestoreAccount), _("Restore from Archive"));
		item->SetBitmap(_icons.GetIconByType(IconType::Undo));
	}
	else {
		item = Append(static_cast<int>(TreeContextMenuType::EditAccount), _("Edit Account..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Pencil));

		item = Append(static_cast<int>(TreeContextMenuType::ArchiveAccount), _("Move to Archive"));
		item->SetBitmap(_icons.GetIconByType(IconType::Delete));

		AppendSeparator();

		if (transactions.size() > 0) {
			wxMenu* recentsMenu = new wxMenu();
			AppendSubMenu(recentsMenu, _("New Transaction"));

			item = recentsMenu->Append(static_cast<int>(TreeContextMenuType::NewTransaction), _("New Transaction..."));
			item->SetBitmap(_icons.GetIconByType(IconType::Plus));

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
			item->SetBitmap(_icons.GetIconByType(IconType::Plus));
		}
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountContextMenu::OnMenuSelect, this);
}

void AccountContextMenu::OnMenuSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id >= transactionsOffset) {
		_commandsInvoker.CopyTransaction(id - transactionsOffset);
	} else {
		switch (static_cast<TreeContextMenuType>(id))
		{
			case TreeContextMenuType::NewTransaction:
				_commandsInvoker.NewTransaction(_account.id);
				break;
			case TreeContextMenuType::EditAccount:
				_commandsInvoker.EditAccount(_account.id);
				break;
			case TreeContextMenuType::NewTab:
				_commandsInvoker.OpenAccountTab(_account.id);
				break;
			case TreeContextMenuType::ArchiveAccount:
				_commandsInvoker.ArchiveAccount(_account.id);
				break;
			case TreeContextMenuType::RestoreAccount:
				_commandsInvoker.RestoreAccount(_account.id);
				break;
			default:
				break;
		}		
	}
}