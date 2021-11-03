#include "TreeContextMenu.h"

TreeContextMenu::TreeContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, AccountViewModel& account, std::vector<std::shared_ptr<TransactionViewModel>> transactions, CommandsInvoker& commandsInvoker): _commandsInvoker(commandsInvoker) {
	this->Append(static_cast<int>(ContextMenuType::NewTab), wxT("Open in New Tab"));

	if (type == TreeMenuItemType::Account) {
		if (parentType == TreeMenuItemType::Archive) {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::RestoreAccount), wxT("Restore from Archive"));

			//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEditAccount, this);
			//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuRestoreAccount, this);
		}
		else {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::DeleteAccount), wxT("Move to Archive"));
			this->AppendSeparator();			

			if (transactions.size() > 0) {
				wxMenu* menuTransaction = new wxMenu();
				this->AppendSubMenu(menuTransaction, wxT("New Transaction"));

				menuTransaction->Append(0, wxT("New Transaction..."));
				menuTransaction->AppendSeparator();
				
				for (auto& transaction : transactions)
				{
					menuTransaction->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));					
				}

				//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnSubMenuAddTransaction, this);
			}
			else {
				this->Append(static_cast<int>(ContextMenuType::NewTransaction), wxT("New Transaction..."));
				//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddTransaction, this);
			}

			//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEditAccount, this);
			//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuDeleteAccount, this);
		}
	}
	else if (type == TreeMenuItemType::Accounts || type == TreeMenuItemType::Receipts || type == TreeMenuItemType::Deposits
		|| type == TreeMenuItemType::Expenses || type == TreeMenuItemType::Credits || type == TreeMenuItemType::Debt) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAccount), wxT("New Account..."));
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::NewTransaction), wxT("New Transaction..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddAccount, this);
		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddTransaction, this);
	}
	else if (type == TreeMenuItemType::Budgets) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddBudget), wxT("New Budget..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddBudget, this);
	}
	else if (type == TreeMenuItemType::Schedulers) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddScheduler), wxT("New Scheduler..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddScheduler, this);
	}
	else if (type == TreeMenuItemType::Goals) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddGoal), wxT("New Goal..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddGoal, this);
	}
	else if (type == TreeMenuItemType::Alerts) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAlert), wxT("New Alert..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddAlert, this);
	}
	else if (type == TreeMenuItemType::Trash) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::EmptyTrash), wxT("Empty Trash..."));

		//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEmptyTrash, this);
	}

	//this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnOpenNewTab, this);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnSubMenuAddTransaction, this);
}

void TreeContextMenu::OnMenuAddAccount(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuEditAccount(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuDeleteAccount(wxCommandEvent& event) {
}

void TreeContextMenu::OnMenuRestoreAccount(wxCommandEvent& event) {
}

void TreeContextMenu::OnMenuNewTransaction(wxCommandEvent& event) {	
	_commandsInvoker.OnNewTransaction(-1);
}

void TreeContextMenu::OnMenuAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(-1);
}

void TreeContextMenu::OnSubMenuAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(event.GetId());
}

void TreeContextMenu::OnMenuAddBudget(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuAddScheduler(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuAddGoal(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuAddAlert(wxCommandEvent& event) {

}

void TreeContextMenu::OnOpenNewTab(wxCommandEvent& event) {
}

void TreeContextMenu::OnMenuEmptyTrash(wxCommandEvent& event) {
}