#include "TreeContextMenu.h"

using namespace Clerk::UI;

TreeContextMenu::TreeContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, std::vector<std::shared_ptr<TransactionViewModel>> transactions, CommandsInvoker& commandsInvoker): _commandsInvoker(commandsInvoker) {
	this->Append(static_cast<int>(ContextMenuType::NewTab), wxT("Open in New Tab"));

	if (type == TreeMenuItemType::Account) {
		if (parentType == TreeMenuItemType::Archive) {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::RestoreAccount), wxT("Restore from Archive"));

			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuType::EditAccount));
			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuRestoreAccount, this, static_cast<int>(ContextMenuType::RestoreAccount));
		}
		else {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::DeleteAccount), wxT("Move to Archive"));
			this->AppendSeparator();			

			if (transactions.size() > 0) {
				wxMenu* menuTransaction = new wxMenu();
				this->AppendSubMenu(menuTransaction, wxT("New Transaction"));

				menuTransaction->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));
				menuTransaction->AppendSeparator();
				
				for (auto& transaction : transactions)
				{
					menuTransaction->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
					this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnSubMenuAddTransaction, this, transaction->id);
				}

				this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnSubMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
			}
			else {
				this->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));
				this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
			}

			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuType::EditAccount));
			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuDeleteAccount, this, static_cast<int>(ContextMenuType::DeleteAccount));
		}
	}
	else if (type == TreeMenuItemType::Accounts || type == TreeMenuItemType::Receipts || type == TreeMenuItemType::Deposits
		|| type == TreeMenuItemType::Expenses || type == TreeMenuItemType::Credits || type == TreeMenuItemType::Debt) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAccount), wxT("New Account..."));
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddAccount, this, static_cast<int>(ContextMenuType::AddAccount));
		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
	}
	else if (type == TreeMenuItemType::Budgets) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddBudget), wxT("New Budget..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddBudget, this, static_cast<int>(ContextMenuType::AddBudget));
	}
	else if (type == TreeMenuItemType::Schedulers) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddScheduler), wxT("New Scheduler..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddScheduler, this, static_cast<int>(ContextMenuType::AddScheduler));
	}
	else if (type == TreeMenuItemType::Goals) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddGoal), wxT("New Goal..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddGoal, this, static_cast<int>(ContextMenuType::AddGoal));
	}
	else if (type == TreeMenuItemType::Alerts) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAlert), wxT("New Alert..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuAddAlert, this, static_cast<int>(ContextMenuType::AddAlert));
	}
	else if (type == TreeMenuItemType::Trash) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::EmptyTrash), wxT("Empty Trash..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnMenuEmptyTrash, this, static_cast<int>(ContextMenuType::EmptyTrash));
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeContextMenu::OnOpenNewTab, this, static_cast<int>(ContextMenuType::NewTab));
}

void TreeContextMenu::OnMenuAddAccount(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuEditAccount(wxCommandEvent& event) {

}

void TreeContextMenu::OnMenuDeleteAccount(wxCommandEvent& event) {
}

void TreeContextMenu::OnMenuRestoreAccount(wxCommandEvent& event) {
}

void TreeContextMenu::OnMenuAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnAddTransaction();
}

void TreeContextMenu::OnSubMenuAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnAddTransaction();
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