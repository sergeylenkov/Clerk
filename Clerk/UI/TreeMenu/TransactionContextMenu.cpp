#include "TransactionContextMenu.h"

const int transactionsOffset = 1000;

TransactionContextMenu::TransactionContextMenu(CommandsInvoker& commandsInvoker, AccountViewModel& account, std::vector<std::shared_ptr<TransactionViewModel>> transactions): TreeContextMenu(commandsInvoker),
	_account(account) {

	this->Append(static_cast<int>(ContextMenuType::NewTab), wxT("Open in New Tab"));
	this->AppendSeparator();

	if (!account.isActive) {	
		this->Append(static_cast<int>(ContextMenuType::RestoreAccount), wxT("Restore from Archive"));
	}
	else {
		this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
		this->Append(static_cast<int>(ContextMenuType::DeleteAccount), wxT("Move to Archive"));
		this->AppendSeparator();

		if (transactions.size() > 0) {
			wxMenu* recentsMenu = new wxMenu();
			this->AppendSubMenu(recentsMenu, wxT("New Transaction"));

			recentsMenu->Append(static_cast<int>(ContextMenuType::NewTransaction), wxT("New Transaction..."));
			recentsMenu->AppendSeparator();

			for (auto& transaction : transactions)
			{
				recentsMenu->Append(transactionsOffset + transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
			}
		}
		else {
			this->Append(static_cast<int>(ContextMenuType::NewTransaction), wxT("New Transaction..."));
		}
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TransactionContextMenu::OnMenuSelect, this);
}

void TransactionContextMenu::OnMenuSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id >= transactionsOffset) {
		_commandsInvoker.OnCopyTransaction(id - transactionsOffset);
	} else if (id == static_cast<int>(ContextMenuType::NewTransaction)) {
		_commandsInvoker.OnNewTransaction(_account.id);
	}
}