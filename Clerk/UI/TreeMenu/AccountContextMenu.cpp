#include "AccountContextMenu.h"

const int transactionsOffset = 1000;

AccountContextMenu::AccountContextMenu(CommandsInvoker& commandsInvoker, AccountPresentationModel& account, std::vector<std::shared_ptr<TransactionPresentationModel>> transactions, Icons& icons): TreeContextMenu(commandsInvoker),
	_account(account), _icons(icons) {

	wxMenuItem* item = this->Append(static_cast<int>(TreeContextMenuType::NewTab), wxT("Open in New Tab"));
	item->SetBitmap(wxBitmap(wxT("ICON_NEW_TAB"), wxBITMAP_TYPE_PNG_RESOURCE));

	this->AppendSeparator();

	if (!account.isActive) {	
		item = this->Append(static_cast<int>(TreeContextMenuType::RestoreAccount), wxT("Restore from Archive"));
		item->SetBitmap(wxBitmap(wxT("ICON_UNDO"), wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		item = this->Append(static_cast<int>(TreeContextMenuType::EditAccount), wxT("Edit Account..."));
		item->SetBitmap(wxBitmap(wxT("ICON_EDIT"), wxBITMAP_TYPE_PNG_RESOURCE));

		item = this->Append(static_cast<int>(TreeContextMenuType::DeleteAccount), wxT("Move to Archive"));
		item->SetBitmap(wxBitmap(wxT("ICON_DELETE"), wxBITMAP_TYPE_PNG_RESOURCE));

		this->AppendSeparator();

		if (transactions.size() > 0) {
			wxMenu* recentsMenu = new wxMenu();
			this->AppendSubMenu(recentsMenu, wxT("New Transaction"));

			item = recentsMenu->Append(static_cast<int>(TreeContextMenuType::NewTransaction), wxT("New Transaction..."));
			item->SetBitmap(wxBitmap(wxT("ICON_ADD"), wxBITMAP_TYPE_PNG_RESOURCE));

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
			item = this->Append(static_cast<int>(TreeContextMenuType::NewTransaction), wxT("New Transaction..."));
			item->SetBitmap(wxBitmap(wxT("ICON_ADD"), wxBITMAP_TYPE_PNG_RESOURCE));
		}
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountContextMenu::OnMenuSelect, this);
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